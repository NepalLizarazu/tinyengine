/*

 Copyright 2019 Moritz Scherer

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "MFCC.h"

#include "main.h"
//#include "profile.h"
#include <stdio.h>
#include <string.h>
#include "float.h"
#include "math.h"
#include "arm_math.h"
#include "arm_math_types.h"
#include <stdlib.h>

int _write(int file, char *ptr, int len) {
	int i = 0;
	for (i = 0; i < len; i++) {
		ITM_SendChar((*ptr++));
	}
	return len;
}

float LogEnergy(uint16_t vectorSize, float *PSDSpectrum) {
	float sum = 0.0f;
	for (int i = 0; i < vectorSize; i++) {
		sum += PSDSpectrum[i];
	}
	if (sum > 0.0f) {
		return (logf(sum / vectorSize));
	} else {
		return (-FLT_MAX);
	}
}

float* CreateDCTMatrix(uint16_t MFCCFeatureNum, int16_t filterBankBinsNum) {
	float *Matrix = (float*) calloc(filterBankBinsNum * MFCCFeatureNum,
			sizeof(float));
	for (int k = 0; k < MFCCFeatureNum; k++) {
		for (int n = 0; n < filterBankBinsNum; n++) {
			Matrix[k * filterBankBinsNum + n] = arm_cos_f32(
					(float) ((PI) / (filterBankBinsNum) * (n + 0.5f) * (k)));
			if (k == 0) {
				Matrix[k * filterBankBinsNum + n] *= sqrt(
						1 / (float) filterBankBinsNum);
			} else {
				Matrix[k * filterBankBinsNum + n] *= sqrt(
						2 / (float) filterBankBinsNum);
			}
		}
	}
	return Matrix;
}

float** CreateMelFilterBank(uint16_t *filterBankFirst, uint16_t *filterBankLast,
		float sampleFrequency, uint16_t windowSize, int16_t filterBankBinsNum) {
	const float frequencyResolution = (sampleFrequency / (windowSize));
	uint16_t leftBin;
	uint16_t centerBin;
	uint16_t rightBin;

	float **melFBank = (float**) calloc(filterBankBinsNum, sizeof(float*));

	for (int i = 0; i < filterBankBinsNum; i++) {

		leftBin = bandBins[i];
		centerBin = bandBins[i + 1];
		rightBin = bandBins[i + 2];
		melFBank[i] = (float*) calloc(rightBin - leftBin, sizeof(float));
		filterBankFirst[i] = leftBin;
		filterBankLast[i] = rightBin;

		// Filternormalization is non-intuitive. Filters are normalized by half the total bandwidth of the triangle.
		// This is matlab specific, normally both halves of the triangle are normalized by their respective bandwidth.

		for (int j = 0; j < rightBin - leftBin; j++) {
			if (centerBin * frequencyResolution > bandEdges[i + 1]) {
				if (leftBin + j < centerBin) {
					melFBank[i][j] = (float) ((leftBin + j)
							* frequencyResolution - bandEdges[i])
							/ ((bandEdges[i + 1] - bandEdges[i])
									* ((bandEdges[i + 2] - bandEdges[i]) / 2));
				} else {
					melFBank[i][j] = (float) (1.000f
							- (((leftBin + j) * frequencyResolution
									- bandEdges[i + 1])
									/ (bandEdges[i + 2] - bandEdges[i + 1])))
							/ ((bandEdges[i + 2] - bandEdges[i]) / 2);
				}
			} else {
				if (leftBin + j <= centerBin) {
					melFBank[i][j] = (float) ((leftBin + j)
							* frequencyResolution - bandEdges[i])
							/ ((bandEdges[i + 1] - bandEdges[i])
									* ((bandEdges[i + 2] - bandEdges[i]) / 2));
				} else {
					melFBank[i][j] = (float) (1.000f
							- (((leftBin + j) * frequencyResolution
									- bandEdges[i + 1])
									/ (bandEdges[i + 2] - bandEdges[i + 1])))
							/ ((bandEdges[i + 2] - bandEdges[i]) / 2);
				}
			}
		}
	}
	return melFBank;
}

void NormalizeVector(uint16_t vectorSize, float *vector) {
	float sumtotal = 0.0f;
	for (int i = 0; i < vectorSize; i++) {
		sumtotal += vector[i] * vector[i];
	}
	if (sumtotal > 0.0f) {
		arm_sqrt_f32(sumtotal, &sumtotal);
		for (int i = 0; i < vectorSize; i++) {
			vector[i] = vector[i] / sumtotal;
		}
	}
}

void MFCCCompute(const int16_t *inputVector, MFCCData data) {

	uint16_t start_i = 0;
	static char fp_buf[20];
	static uint8_t power = 1; //1 if power of the FFT is used, 0 if only the magnitude is used

	if (data.fft_length != data.windowSize) { //Pads with zero if the input shape is not the same as the fft_length
		uint16_t diff = data.fft_length - data.windowSize;
		start_i = diff / 2;
		for (uint8_t i = 0; i < diff / 2; i++) {
			data.dataWindow[i] = 0;
			data.dataWindow[data.fft_length - 1 - i] = 0;
		}
	}

	for (int i = 0; i < data.windowSize; i++) {
		data.dataWindow[i + start_i] = ((((float) inputVector[i]
				* data.windowFunction[i])));
	}

	arm_rfft_fast_f32(data.RFFTInstance, data.dataWindow, data.PSDSpectrum, 0);

	data.PSDSpectrum[0] = (data.PSDSpectrum[0] * data.PSDSpectrum[0]);

	float temp = data.PSDSpectrum[1];

	for (int i = 1; i < data.fft_length / 2; i++) {
		data.PSDSpectrum[i] = (data.PSDSpectrum[2 * i] * data.PSDSpectrum[2 * i]
				+ data.PSDSpectrum[2 * i + 1] * data.PSDSpectrum[2 * i + 1]);
	}

	for (int i = 1; i < data.fft_length / 2; i++) {
		data.PSDSpectrum[data.fft_length - i] = data.PSDSpectrum[i];
	}

	data.PSDSpectrum[data.fft_length / 2] = (temp * temp);

	// Keep everything squared for log energy.


	*data.logEnergy = (LogEnergy(data.fft_length, data.PSDSpectrum) + 1); // Account for numeric differences with +1 term


	if (power == 0) {
		// Calculate as few square roots as necessary -> windowSize/2 + 1
		arm_sqrt_f32(data.PSDSpectrum[0], &data.PSDSpectrum[0]);
		arm_sqrt_f32(data.PSDSpectrum[data.fft_length / 2],
				&data.PSDSpectrum[data.fft_length / 2]);

		for (int i = 1; i < data.fft_length / 2; i++) {
			arm_sqrt_f32(data.PSDSpectrum[i], &data.PSDSpectrum[i]);
			data.PSDSpectrum[data.fft_length - i] = data.PSDSpectrum[i];
		}

	}

//	for (int i = 0; i < (513); i++){
//		sprintf(fp_buf, "%d, ", (int)(data.PSDSpectrum[i]) );
//		printLog(fp_buf);
//	}

	float melEnergy;
	int j;
	for (int bin = 0; bin < data.filterBankBinsNum; bin++) {
		j = 0;
		melEnergy = 0.0f;
		int16_t firstIndex = data.filterBankFirst[bin];
		int16_t lastIndex = data.filterBankLast[bin];
		for (int i = firstIndex; i <= lastIndex; i++) {
			melEnergy += data.PSDSpectrum[i] * data.melFilterBank[bin][j++];
		}
		data.melEnergies[bin] = melEnergy;
	}

	for (int bin = 0; bin < data.filterBankBinsNum; bin++) {
		data.melEnergies[bin] = logf(data.melEnergies[bin] + 1e-6);
	}

//	for (int i = 0; i < (40); i++){
//		sprintf(fp_buf, "%d, ", (int)(data.melEnergies[i]) );
//		printLog(fp_buf);
//	}

	for (int i = 0; i < data.MFCCFeatureNum; i++) {
		float sum = 0.0f;
		for (int j = 0; j < data.filterBankBinsNum; j++) {
			sum += data.DCTMatrix[i * data.filterBankBinsNum + j]
					* data.melEnergies[j];
		}
		sum += 128.; //In account for quantization
		if (sum > 255.) {
			sum = 255.;
		} else if (sum < 0.) {
			sum = 0.;
		}
		data.melCoefficients[i] = sum;

	}

//	for (int i = 0; i < (10); i++){
//		sprintf(fp_buf, "%d, ", (int)(data.melCoefficients[i]) );
//		printLog(fp_buf);
//	}

}
MFCCData MFCCInitialize(uint16_t windowSize, float sampleFrequency,
		float MFCCFeatureNum) {

	const uint16_t fft_len = 1024;
	const float frequencyResolution = (sampleFrequency / (fft_len));
	int16_t filterBankBinsNum = 40;
//	float factor = 133.33333333333333f;

	static char fp_buf[10]; //Remove

//	float low_freq_mel = 0.0f;
//	float high_freq_mel = 2595.0f * log10f(1.0f + (sampleFrequency / 4.0f) / 700.0f); //4KHz (Fs/4)
//	float* mel_points = (float*)calloc(filterBankBinsNum + 2, sizeof(float));
//	// Equally spaced in Mel scale
//	for (int i = 0; i < filterBankBinsNum + 2; i++) {
//		mel_points[i] = low_freq_mel + (high_freq_mel - low_freq_mel) * i / (filterBankBinsNum + 1);
//	}
//
//	// Convert Mel to Hz
//	for (int i = 0; i < filterBankBinsNum + 2; i++) {
//		float mel = mel_points[i];
//		float hz = 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
//		bandEdges[i] = hz;
//	}
//
//	free(mel_points);
//	for (int i = 0; i < 13; i++) {
//		bandEdges[i] = factor + i * (factor / 2);
//		if (bandEdges[i] <= sampleFrequency / 4) { //Used to be /2, but to match the python code we use /4 so its 4KHz
//			filterBankBinsNum = i - 1;
//		}
//	}
//
//	for (int i = 13; i < 42; i++) {
//		bandEdges[i] = bandEdges[i - 1] * 1.0711703f;
//		if (bandEdges[i] <= sampleFrequency / 4) {
//			filterBankBinsNum = i - 1;
//		}
//	}
//	for (int i = 0; i < 42; i++) {
//		bandBins[i] = (uint16_t) ceil(bandEdges[i] / frequencyResolution);
//	}

	float *PSDSpectrum = (float*) calloc(fft_len, sizeof(float));
	float *dataWindow = (float*) calloc(fft_len, sizeof(float));
	float *melEnergies = (float*) calloc(filterBankBinsNum, sizeof(float));
//	uint16_t *filterBankFirst = (uint16_t*) calloc(filterBankBinsNum,
//			sizeof(uint16_t));//Defined in the Header file
//	uint16_t *filterBankLast = (uint16_t*) calloc(filterBankBinsNum,
//			sizeof(uint16_t));//Defined in the Header file
	arm_rfft_fast_instance_f32 *RFFTInstance =
			(arm_rfft_fast_instance_f32*) calloc(1,
					sizeof(arm_rfft_fast_instance_f32));

	float *featureVector = (float*) calloc(MFCCFeatureNum - 1 + 2,
			sizeof(float));

	float *windowFunction = HammingWindow(windowSize);
	arm_rfft_fast_init_f32(RFFTInstance, fft_len);
	float *DCTMatrix = CreateDCTMatrix(MFCCFeatureNum, filterBankBinsNum);
//	float **melFilterBank = CreateMelFilterBank(filterBankFirst, filterBankLast,
//			sampleFrequency, fft_len, filterBankBinsNum); //Defined in the Header file

	MFCCData data = { windowSize : windowSize, sampleFrequency :
	sampleFrequency, MFCCFeatureNum : MFCCFeatureNum, filterBankBinsNum
			: filterBankBinsNum,

	logEnergy : calloc(1, sizeof(float)),

	PSDSpectrum : PSDSpectrum, dataWindow : dataWindow, melEnergies :
	melEnergies, filterBankFirst : filterBankFirst, filterBankLast
			: filterBankLast, RFFTInstance : RFFTInstance, windowFunction
			: windowFunction, DCTMatrix : DCTMatrix, melFilterBank
			: (float**) melFilterBanks, melCoefficients : (float*) calloc(
			MFCCFeatureNum, sizeof(float)), fft_length : fft_len, };
	return data;
}

float* HammingWindow(int windowSize) {
	float *ret = (float*) calloc(windowSize, sizeof(float));
	for (int i = 0; i < windowSize; i++) {
		//ret[i] = 0.54f - 0.46f * cos(2 * PI * i / (windowSize - 1)); //Hamming window
		ret[i] = 0.5f * (1 - cos(2 * PI * i / (windowSize - 1))); //Hanning Window
	}
	return ret;
}

void MapMinMax(float *vector, uint32_t vectorSize, float ymin, float ymax) {
	float xmin = vector[0];
	float xmax = vector[0];

	for (int i = 0; i < vectorSize; i++) {
		if (vector[i] > xmax) {
			xmax = vector[i];
		} else if (vector[i] < xmin) {
			xmin = vector[i];
		}
	}
	float norm = (ymax - ymin) / (xmax - xmin);
	for (int i = 0; i < vectorSize; i++) {
		vector[i] = (norm * ((vector[i]) - xmin) + ymin);
	}

}

