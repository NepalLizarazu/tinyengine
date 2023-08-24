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

#ifndef MFCC_H
#define MFCC_H

#define ARM_MATH_NEON
#include "arm_math.h"
#include "arm_neon.h"
#include "string.h"
#include "stdio.h"
#include "main.h"
static float bandEdges[42];
static uint16_t bandBins[42];

// Contains all necessary data to compute MFCC coefficients for given input
typedef struct {
	float sampleFrequency;
	uint16_t MFCCFeatureNum;
	uint16_t filterBankBinsNum;
	float* logEnergy;
	float* melCoefficients;
	float lowerBandEdge;
	float upperBandEdge;
	uint16_t windowSize;
	float* dataWindow;
	float* PSDSpectrum;
	float* melEnergies;
	float* windowFunction;
	float** melFilterBank;
	float* DCTMatrix;
	uint16_t* filterBankFirst;
	uint16_t* filterBankLast;
	arm_rfft_fast_instance_f32* RFFTInstance;
	uint16_t fft_length;
} MFCCData;

//Mel filters for 16KHz, 1024 FFT length
const float melfilter0[4] = {0.33882982,0.8008822,0.7466324,0.30332568};
const float melfilter1[5] = {0.2533676,0.6966743,0.8688354,0.4428124,0.024936508};
const float melfilter2[5] = {0.13116463,0.5571876,0.9750635,0.6149026,0.21241836};
const float melfilter3[5] = {0.38509738,0.7875816,0.8172138,0.42902985,0.047619484};
const float melfilter4[5] = {0.18278626,0.5709702,0.95238054,0.6727508,0.3042046};
const float melfilter5[5] = {0.32724923,0.69579536,0.94177043,0.5852521,0.2344533};
const float melfilter6[6] = {0.058229554,0.41474792,0.7655467,0.8891983,0.5493126,0.21463157};
const float melfilter7[6] = {0.110801674,0.4506874,0.78536844,0.88499784,0.5602629,0.24028413};
const float melfilter8[7] = {0.11500214,0.43973708,0.75971586,0.9249212,0.6140458,0.30753008,0.005253839};
const float melfilter9[7] = {0.07507876,0.38595417,0.69246995,0.99474615,0.7070996,0.41296002,0.12272859};
const float melfilter10[6] = {0.29290038,0.58703995,0.8772714,0.83629775,0.5535752,0.2744625};
const float melfilter11[7] = {0.16370223,0.4464248,0.72553754,0.9988685,0.72670805,0.4578947,0.19234921};
const float melfilter12[8] = {0.0011314868,0.27329195,0.54210526,0.8076508,0.92998976,0.67074287,0.41453537,0.16129743};
const float melfilter13[8] = {0.07001024,0.32925713,0.58546466,0.83870256,0.9109593,0.6634561,0.41872823,0.17670368};
const float melfilter14[9] = {0.089040674,0.33654395,0.58127177,0.8232963,0.9373362,0.70056075,0.4663217,0.23456573,0.005243187};
const float melfilter15[9] = {0.06266378,0.29943922,0.5336783,0.76543427,0.9947568,0.77830434,0.55369353,0.33137178,0.11128694};
const float melfilter16[9] = {0.22169566,0.44630647,0.6686282,0.88871306,0.89339644,0.6776565,0.4640256,0.25246477,0.042929035};
const float melfilter17[10] = {0.10660358,0.32234356,0.53597444,0.7475352,0.95707095,0.8353862,0.6297951,0.42611772,0.22432226,0.024373155};
const float melfilter18[10] = {0.16461377,0.37020493,0.5738823,0.77567774,0.9756268,0.8262389,0.62988234,0.4352763,0.24238881,0.0511867};
const float melfilter19[10] = {0.17376113,0.37011766,0.5647237,0.7576112,0.9488133,0.8616411,0.67372626,0.48741633,0.30267817,0.1194905};
const float melfilter20[11] = {0.13835892,0.32627374,0.5125837,0.69732183,0.8805095,0.937825,0.75765824,0.5789591,0.4017157,0.22589968,0.051480282};
const float melfilter21[12] = {0.062174965,0.24234176,0.42104092,0.5982843,0.7741003,0.9485197,0.8784456,0.70676976,0.53643125,0.36740893,0.19968848,0.033241566};
const float melfilter22[12] = {0.12155435,0.29323027,0.46356872,0.63259107,0.8003115,0.96675843,0.8680516,0.70410675,0.54138094,0.37985292,0.21951318,0.06034044};
const float melfilter23[12] = {0.13194841,0.29589328,0.45861906,0.6201471,0.7804868,0.93965954,0.9023181,0.7454344,0.5896656,0.4349975,0.28142065,0.12891375};
const float melfilter24[13] = {0.09768187,0.2545656,0.4103344,0.5650025,0.71857935,0.87108624,0.9774602,0.82705295,0.67767537,0.52930623,0.38194308,0.2355623,0.09015914};
const float melfilter25[14] = {0.022539785,0.17294705,0.3223246,0.47069377,0.6180569,0.7644377,0.9098409,0.94571465,0.8022194,0.65966153,0.5180245,0.3773035,0.23747967,0.09854351};
const float melfilter26[15] = {0.05428533,0.19778058,0.34033844,0.4819755,0.6226965,0.7625203,0.9014565,0.96048784,0.8232983,0.68696797,0.551478,0.4168237,0.28299546,0.14997917,0.017770067};
const float melfilter27[15] = {0.03951218,0.17670172,0.313032,0.44852197,0.5831763,0.71700454,0.8500208,0.98222995,0.8863542,0.7557243,0.62587315,0.49678662,0.36845756,0.2408765,0.11403872};
const float melfilter28[16] = {0.11364578,0.24427569,0.37412682,0.50321335,0.63154244,0.7591235,0.8859613,0.98793,0.862548,0.7378809,0.6139215,0.49065813,0.36808828,0.24620254,0.12499142,0.0044478322};
const float melfilter29[17] = {0.012069982,0.13745198,0.2621191,0.38607845,0.5093419,0.6319117,0.7537975,0.8750086,0.9955522,0.884567,0.76534194,0.6467631,0.52882093,0.41151324,0.29483515,0.17877255,0.06332539};
const float melfilter30[17] = {0.11543296,0.23465806,0.3532369,0.47117904,0.5884868,0.70516485,0.82122743,0.9366746,0.94848657,0.8342443,0.72059375,0.6075374,0.49506098,0.3831598,0.27182907,0.16106407,0.05085536};
const float melfilter31[18] = {0.051513422,0.16575572,0.27940622,0.39246258,0.504939,0.61684024,0.72817093,0.8389359,0.94914466,0.94119585,0.8320926,0.72352666,0.6154957,0.5079997,0.40102923,0.29457715,0.18863873,0.08321872};
const float melfilter32[19] = {0.058804173,0.16790743,0.27647337,0.38450432,0.49200028,0.5989708,0.7054229,0.81136125,0.91678125,0.9783006,0.8738842,0.7699673,0.66653806,0.5635988,0.46113768,0.35915947,0.2576547,0.15661624,0.05604647};
const float melfilter33[20] = {0.021699455,0.1261158,0.2300327,0.33346194,0.43640122,0.5388623,0.64084053,0.7423453,0.8433838,0.9439535,0.95593596,0.85627985,0.7570759,0.6583241,0.5600126,0.46214372,0.3647104,0.26771027,0.17113857,0.07499059};
const float melfilter34[21] = {0.044064075,0.14372014,0.24292406,0.34167588,0.43998742,0.5378563,0.6352896,0.73228973,0.8288614,0.9250094,0.97926396,0.8839563,0.78906053,0.6945767,0.60049754,0.50682324,0.41355133,0.32066998,0.22818632,0.13609087,0.04438127};
const float melfilter35[22] = {0.020736035,0.11604368,0.21093944,0.30542332,0.39950243,0.49317676,0.58644867,0.67933,0.7718137,0.8639091,0.95561874,0.953055,0.8621074,0.7715409,0.6813437,0.5915204,0.5020618,0.41297236,0.32424274,0.23587058,0.14785585,0.060191453};
const float melfilter36[23] = {0.04694498,0.13789259,0.22845909,0.31865633,0.40847954,0.49793822,0.5870276,0.6757572,0.7641294,0.8521441,0.93980855,0.9728798,0.8859139,0.79929364,0.71301425,0.62707806,0.5414756,0.45620698,0.3712697,0.28666148,0.20237991,0.11842264,0.034784935};
const float melfilter37[24] = {0.027120177,0.11408606,0.20070635,0.28698578,0.37292194,0.45852438,0.543793,0.6287303,0.71333855,0.7976201,0.8815774,0.9652151,0.9514668,0.86846584,0.7857797,0.7034013,0.6213378,0.5395772,0.45812196,0.3769721,0.29612052,0.2155648,0.13530736,0.055343434};
const float melfilter38[25] = {0.04853321,0.13153416,0.21422029,0.29659867,0.37866223,0.4604228,0.54187804,0.62302786,0.7038795,0.7844352,0.8646926,0.94465655,0.9756683,0.89628434,0.81718916,0.738378,0.6598509,0.58160317,0.50363946,0.4259456,0.34853107,0.27139112,0.19452105,0.11792081,0.04158569};
const float melfilter39[26] = {0.0243317,0.10371568,0.18281087,0.261622,0.34014907,0.41839683,0.49636054,0.5740544,0.65146893,0.72860885,0.80547893,0.8820792,0.9584143,0.9655203,0.88971525,0.8141706,0.73889107,0.6638671,0.5890989,0.51458627,0.4403293,0.36632326,0.29256815,0.21905921,0.14579645,0.07277513};
const uint16_t filterBankFirst[40] = {2, 4, 6, 9, 11, 14, 16, 19, 22, 25, 29, 32, 35, 39, 43, 47, 52, 56, 61, 66, 71, 76, 82, 88, 94, 100, 107, 114, 122, 129, 138, 146, 155, 164, 174, 184, 195, 206, 218, 230};
const uint16_t filterBankLast[40] = {5, 8, 10, 13, 15, 18, 21, 24, 28, 31, 34, 38, 42, 46, 51, 55, 60, 65, 70, 75, 81, 87, 93, 99, 106, 113, 121, 128, 137, 145, 154, 163, 173, 183, 194, 205, 217, 229, 242, 255};
static const float* melFilterBanks[] = {&melfilter0[0], &melfilter1[0], &melfilter2[0], &melfilter3[0], &melfilter4[0], &melfilter5[0], &melfilter6[0], &melfilter7[0], &melfilter8[0], &melfilter9[0], &melfilter10[0], &melfilter11[0], &melfilter12[0], &melfilter13[0], &melfilter14[0], &melfilter15[0], &melfilter16[0], &melfilter17[0], &melfilter18[0], &melfilter19[0], &melfilter20[0], &melfilter21[0], &melfilter22[0], &melfilter23[0], &melfilter24[0], &melfilter25[0], &melfilter26[0], &melfilter27[0], &melfilter28[0], &melfilter29[0], &melfilter30[0], &melfilter31[0], &melfilter32[0], &melfilter33[0], &melfilter34[0], &melfilter35[0], &melfilter36[0], &melfilter37[0], &melfilter38[0], &melfilter39[0]};

// Creates a DCT-II Matrix with the dimensions given as arguments.
// This function is called from within MFCCInitialize.
float* CreateDCTMatrix(uint16_t MFCCFeatureNum, int16_t filterBankBinsNum);
// Returns array of size MFCCFeatureNum x filterBankBinsNum

// Creates the filterbanks which lie within the given sample frequency.
// This function is called from within MFCCInitialize.
float** CreateMelFilterBank(uint16_t* filterBankFirst, uint16_t* filterBankLast, float sampleFrequency, uint16_t windowSize, int16_t filterBankBinsNum);
// Returns array with variable size, depending on sampleFrequency.

// This function processes a given input vector of sample data and returns
// the results in data.melCoefficients.
// To generate an appropriate MFCCData struct, call MFCCInitialize.
// Due to optimized algorithms, some numerical discrepancies are to be expected, but should be in the order of 1%
void MFCCCompute(const int16_t* inputVector, MFCCData data);
// Returns void. Results are saved in data.melCoefficients

// Initialize all necessary data fields and structs to call MFCCCompute.
// This needs to be called once to generate the MFCCData struct
MFCCData MFCCInitialize(uint16_t windowSize, float sampleFrequency, float MFCCFeatureNum);
// Returns the data struct needed for MFCCCompute

// Normalizes a given vector with its euclidean norm.
void NormalizeVector(uint16_t vectorSize, float* vector);

// This function calculates the hamming window coefficients for a given window size.
float* HammingWindow(int windowSize);
// Returns a vector of size windowSize with the coefficients of the hamming window

// Calculates the logarithmic energy for a given PSD vector
// Used in the calculation of MFCCCCompute
float LogEnergy(uint16_t vectorSize, float* PSDSpectrum);
// Returns a single float with the log energy of the given vector

// Maps the vector linearly such that the maximum value is ymax and the minimum value is ymin.
// Used to normalize the input of the data (Faster than normalization, but most often better)
void MapMinMax(float* vector, uint32_t vectorSize, float ymin, float ymax);
// No return. The vector is normalized.

#endif

