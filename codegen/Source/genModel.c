/* Automatically generated source file */
#include <float.h>
#include "arm_nnfunctions.h"

#include "genNN.h"
#include "genModel.h"

#include "tinyengine_function.h"
#include "tinyengine_function_fp.h"


/* Variables used by all ops */
ADD_params add_params;
//Conv_Params conv_params;
//Depthwise_Params dpconv_params;
int i;
int8_t *int8ptr,*int8ptr2;
int32_t *int32ptr;
float *fptr,*fptr2,*fptr3;

signed char* getInput() {
    return &buffer0[8000];
}
signed char* getOutput() {
    return NNoutput;
}
void end2endinference(q7_t* img){
    invoke(NULL);
}
void invoke(float* labels){
/* layer 0:CONV_2D */
convolve_s8_kernel3_inputch1_stride2_pad1_fpreq(&buffer0[8000],10,49,1,(const q7_t*) weight0,bias0,scales0,-128,122,-128,127,&buffer0[0],5,25,64,sbuf,kbuf,-122);
/* layer 1:DEPTHWISE_CONV_2D */
depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0],5,25,64,(const q7_t*) CHWweight1,offsetBias1,offsetRBias1,scales1,-128,128,-128,127,&buffer0[0],5,25,64,sbuf,-128);
/* layer 2:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[0],5,25,64,(const q7_t*) weight2,bias2,scales2,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf);
/* layer 3:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[8000],5,25,64,(const q7_t*) CHWweight3,offsetBias3,offsetRBias3,scales3,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf,-128);
/* layer 4:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[8000],5,25,64,(const q7_t*) weight4,bias4,scales4,-128,128,-128,127,&buffer0[0],5,25,64,sbuf);
/* layer 5:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0],5,25,64,(const q7_t*) CHWweight5,offsetBias5,offsetRBias5,scales5,-128,128,-128,127,&buffer0[0],5,25,64,sbuf,-128);
/* layer 6:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[0],5,25,64,(const q7_t*) weight6,bias6,scales6,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf);
/* layer 7:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[8000],5,25,64,(const q7_t*) CHWweight7,offsetBias7,offsetRBias7,scales7,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf,-128);
/* layer 8:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[8000],5,25,64,(const q7_t*) weight8,bias8,scales8,-128,128,-128,127,&buffer0[16000],5,25,64,sbuf);
/* layer 9:AVERAGE_POOL_2D */
avg_pooling(&buffer0[16000],25,5,64,25,5,1,1,-128,127,&buffer0[0]);
/* layer 10:CONV_2D */
convolve_1x1_s8_fpreq_bitmask(&buffer0[0],1,1,64,(const q7_t*) weight9,bias9,scales9,4,128,-128,127,&buffer0[68],&buffer0[64],1,1,10,sbuf);
/* layer 11:CAST */
fptr = (float*)&buffer0[108];
int8ptr = (int8_t*)&buffer0[68];
for(int i = 0; i < 10; i++) fptr[i] = (float)int8ptr[i];
const float last_y_scale = 0.20769637823104858;
/* layer 12:LOG_SOFTMAX */
LogSoftmax((float*)&buffer0[108],1,1,10,(float*)&buffer0[68],1,1,10);
/* layer 13:NLL_LOSS */
nll_loss((float*)&buffer0[68],2,1,labels,10,(float*)&buffer0[108]);
/* layer 14:ONES */
fptr = (float*)&buffer0[108];for(int i = 0; i < 1; i++) fptr[i] = 1.0;
/* layer 15:NEGATIVE */
fptr = (float*)&buffer0[108];fptr2 = (float*)&buffer0[148];for(int i = 0; i < 1; i++) fptr2[i] = fptr[i] * -1.0f;
/* layer 16:MUL */
fptr = (float*)labels;fptr3 = (float*)&buffer0[108];fptr2 = &buffer0[148];for(int i = 0; i < 10; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 17:SUM */
sum_3D((float*)&buffer0[108],1,10,1,1,(float*)&buffer0[148]);
/* layer 18:EXP */
tte_exp(10,(float*)&buffer0[68],(float*)&buffer0[152]);
/* layer 19:MUL */
fptr = (float*)&buffer0[152];fptr3 = (float*)&buffer0[192];fptr2 = &buffer0[148];for(int i = 0; i < 10; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 20:SUBTRACT */
sub(10,(float*)&buffer0[108],(float*)&buffer0[192],(float*)&buffer0[68]);
/* layer 21:WHERE */
where_zeros_inplace_bit((bool*)&buffer0[64],10,(float*)&buffer0[68]);
/* layer 22:MUL */
mul(10,(float*)&buffer0[68],scales9,(float*)&buffer0[108]);
/* layer 23:BIAS_UPDATE */
const float v5_bias_QAS[10] = {75942980.0,66743132.0,91431460.0,88748140.0,53406920.0,67439176.0,68025480.0,85778140.0,63799956.0,89537280.0,};
fptr = (float*)&buffer0[108];//gradients
for(int i = 0; i < 10; i++) v5_bias[i] -= (int32_t)(fptr[i] * blr * v5_bias_QAS[i]);
/* layer 24:GROUP_CONV */
const float v5_weight_QAS[10] = {27141.816,23853.818,32677.361,31718.354,19087.49,24102.582,24312.13,30656.88,22801.936,32000.387,};
group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight_int8input_inplace((float*)&buffer0[0],1,1,64,weight_fp10,NULL,v5_weight,1,1,640,-FLT_MAX,FLT_MAX,(float*)sbuf,1,64, v5_weight_QAS, lr);
}
void invoke_inf(){
/* layer 0:CONV_2D */
convolve_s8_kernel3_inputch1_stride2_pad1_fpreq(&buffer0[8000],10,49,1,(const q7_t*) weight0,bias0,scales0,-128,122,-128,127,&buffer0[0],5,25,64,sbuf,kbuf,-122);
/* layer 1:DEPTHWISE_CONV_2D */
depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0],5,25,64,(const q7_t*) CHWweight1,offsetBias1,offsetRBias1,scales1,-128,128,-128,127,&buffer0[0],5,25,64,sbuf,-128);
/* layer 2:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[0],5,25,64,(const q7_t*) weight2,bias2,scales2,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf);
/* layer 3:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[8000],5,25,64,(const q7_t*) CHWweight3,offsetBias3,offsetRBias3,scales3,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf,-128);
/* layer 4:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[8000],5,25,64,(const q7_t*) weight4,bias4,scales4,-128,128,-128,127,&buffer0[0],5,25,64,sbuf);
/* layer 5:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0],5,25,64,(const q7_t*) CHWweight5,offsetBias5,offsetRBias5,scales5,-128,128,-128,127,&buffer0[0],5,25,64,sbuf,-128);
/* layer 6:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[0],5,25,64,(const q7_t*) weight6,bias6,scales6,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf);
/* layer 7:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[8000],5,25,64,(const q7_t*) CHWweight7,offsetBias7,offsetRBias7,scales7,-128,128,-128,127,&buffer0[8000],5,25,64,sbuf,-128);
/* layer 8:CONV_2D */
convolve_1x1_s8_fpreq(&buffer0[8000],5,25,64,(const q7_t*) weight8,bias8,scales8,-128,128,-128,127,&buffer0[16000],5,25,64,sbuf);
/* layer 9:AVERAGE_POOL_2D */
avg_pooling(&buffer0[16000],25,5,64,25,5,1,1,-128,127,&buffer0[0]);
/* layer 10:CONV_2D */
convolve_1x1_s8_fpreq_bitmask(&buffer0[0],1,1,64,(const q7_t*) weight9,bias9,scales9,4,128,-128,127,&buffer0[68],&buffer0[64],1,1,10,sbuf);
}
