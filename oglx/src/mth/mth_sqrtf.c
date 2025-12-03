/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_sqrtf.c
 DESCRIPTION       : Function used to compute the square root of a float
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl_types.h"

/*+ Protected interfaces +*/
#include "mth.h"

/******************************************************************************
 **                                   Local file declarations
 *****************************************************************************/


/*
** Local static table
*/
static const SGLfloat stc_tf_tabforsqrt[512U] = {
    (SGLfloat) 7.057310819e-01F, /*[  0] */ (SGLfloat) 3.542412403e-01F,        /*[  0] */
    (SGLfloat) 7.030009503e-01F, /*[  1] */ (SGLfloat) 3.556169706e-01F,        /*[  1] */
    (SGLfloat) 7.003022603e-01F, /*[  2] */ (SGLfloat) 3.569873991e-01F,        /*[  2] */
    (SGLfloat) 6.976344131e-01F, /*[  3] */ (SGLfloat) 3.583525866e-01F,        /*[  3] */
    (SGLfloat) 6.949968255e-01F, /*[  4] */ (SGLfloat) 3.597125927e-01F,        /*[  4] */
    (SGLfloat) 6.923889299e-01F, /*[  5] */ (SGLfloat) 3.610674759e-01F,        /*[  5] */
    (SGLfloat) 6.898101733e-01F, /*[  6] */ (SGLfloat) 3.624172938e-01F,        /*[  6] */
    (SGLfloat) 6.872600171e-01F, /*[  7] */ (SGLfloat) 3.637621028e-01F,        /*[  7] */
    (SGLfloat) 6.847379365e-01F, /*[  8] */ (SGLfloat) 3.651019581e-01F,        /*[  8] */
    (SGLfloat) 6.822434200e-01F, /*[  9] */ (SGLfloat) 3.664369142e-01F,        /*[  9] */
    (SGLfloat) 6.797759693e-01F, /*[ 10] */ (SGLfloat) 3.677670243e-01F,        /*[ 10] */
    (SGLfloat) 6.773350984e-01F, /*[ 11] */ (SGLfloat) 3.690923410e-01F,        /*[ 11] */
    (SGLfloat) 6.749203334e-01F, /*[ 12] */ (SGLfloat) 3.704129155e-01F,        /*[ 12] */
    (SGLfloat) 6.725312124e-01F, /*[ 13] */ (SGLfloat) 3.717287986e-01F,        /*[ 13] */
    (SGLfloat) 6.701672846e-01F, /*[ 14] */ (SGLfloat) 3.730400398e-01F,        /*[ 14] */
    (SGLfloat) 6.678281104e-01F, /*[ 15] */ (SGLfloat) 3.743466879e-01F,        /*[ 15] */
    (SGLfloat) 6.655132607e-01F, /*[ 16] */ (SGLfloat) 3.756487909e-01F,        /*[ 16] */
    (SGLfloat) 6.632223168e-01F, /*[ 17] */ (SGLfloat) 3.769463958e-01F,        /*[ 17] */
    (SGLfloat) 6.609548702e-01F, /*[ 18] */ (SGLfloat) 3.782395489e-01F,        /*[ 18] */
    (SGLfloat) 6.587105218e-01F, /*[ 19] */ (SGLfloat) 3.795282959e-01F,        /*[ 19] */
    (SGLfloat) 6.564888822e-01F, /*[ 20] */ (SGLfloat) 3.808126813e-01F,        /*[ 20] */
    (SGLfloat) 6.542895709e-01F, /*[ 21] */ (SGLfloat) 3.820927492e-01F,        /*[ 21] */
    (SGLfloat) 6.521122165e-01F, /*[ 22] */ (SGLfloat) 3.833685428e-01F,        /*[ 22] */
    (SGLfloat) 6.499564559e-01F, /*[ 23] */ (SGLfloat) 3.846401047e-01F,        /*[ 23] */
    (SGLfloat) 6.478219346e-01F, /*[ 24] */ (SGLfloat) 3.859074767e-01F,        /*[ 24] */
    (SGLfloat) 6.457083062e-01F, /*[ 25] */ (SGLfloat) 3.871706999e-01F,        /*[ 25] */
    (SGLfloat) 6.436152320e-01F, /*[ 26] */ (SGLfloat) 3.884298149e-01F,        /*[ 26] */
    (SGLfloat) 6.415423809e-01F, /*[ 27] */ (SGLfloat) 3.896848614e-01F,        /*[ 27] */
    (SGLfloat) 6.394894295e-01F, /*[ 28] */ (SGLfloat) 3.909358787e-01F,        /*[ 28] */
    (SGLfloat) 6.374560614e-01F, /*[ 29] */ (SGLfloat) 3.921829053e-01F,        /*[ 29] */
    (SGLfloat) 6.354419671e-01F, /*[ 30] */ (SGLfloat) 3.934259791e-01F,        /*[ 30] */
    (SGLfloat) 6.334468441e-01F, /*[ 31] */ (SGLfloat) 3.946651375e-01F,        /*[ 31] */
    (SGLfloat) 6.314703964e-01F, /*[ 32] */ (SGLfloat) 3.959004173e-01F,        /*[ 32] */
    (SGLfloat) 6.295123345e-01F, /*[ 33] */ (SGLfloat) 3.971318546e-01F,        /*[ 33] */
    (SGLfloat) 6.275723751e-01F, /*[ 34] */ (SGLfloat) 3.983594852e-01F,        /*[ 34] */
    (SGLfloat) 6.256502410e-01F, /*[ 35] */ (SGLfloat) 3.995833440e-01F,        /*[ 35] */
    (SGLfloat) 6.237456608e-01F, /*[ 36] */ (SGLfloat) 4.008034657e-01F,        /*[ 36] */
    (SGLfloat) 6.218583690e-01F, /*[ 37] */ (SGLfloat) 4.020198843e-01F,        /*[ 37] */
    (SGLfloat) 6.199881055e-01F, /*[ 38] */ (SGLfloat) 4.032326332e-01F,        /*[ 38] */
    (SGLfloat) 6.181346159e-01F, /*[ 39] */ (SGLfloat) 4.044417456e-01F,        /*[ 39] */
    (SGLfloat) 6.162976509e-01F, /*[ 40] */ (SGLfloat) 4.056472539e-01F,        /*[ 40] */
    (SGLfloat) 6.144769665e-01F, /*[ 41] */ (SGLfloat) 4.068491901e-01F,        /*[ 41] */
    (SGLfloat) 6.126723235e-01F, /*[ 42] */ (SGLfloat) 4.080475858e-01F,        /*[ 42] */
    (SGLfloat) 6.108834877e-01F, /*[ 43] */ (SGLfloat) 4.092424722e-01F,        /*[ 43] */
    (SGLfloat) 6.091102298e-01F, /*[ 44] */ (SGLfloat) 4.104338799e-01F,        /*[ 44] */
    (SGLfloat) 6.073523249e-01F, /*[ 45] */ (SGLfloat) 4.116218391e-01F,        /*[ 45] */
    (SGLfloat) 6.056095528e-01F, /*[ 46] */ (SGLfloat) 4.128063795e-01F,        /*[ 46] */
    (SGLfloat) 6.038816976e-01F, /*[ 47] */ (SGLfloat) 4.139875305e-01F,        /*[ 47] */
    (SGLfloat) 6.021685477e-01F, /*[ 48] */ (SGLfloat) 4.151653211e-01F,        /*[ 48] */
    (SGLfloat) 6.004698956e-01F, /*[ 49] */ (SGLfloat) 4.163397797e-01F,        /*[ 49] */
    (SGLfloat) 5.987855381e-01F, /*[ 50] */ (SGLfloat) 4.175109345e-01F,        /*[ 50] */
    (SGLfloat) 5.971152758e-01F, /*[ 51] */ (SGLfloat) 4.186788132e-01F,        /*[ 51] */
    (SGLfloat) 5.954589132e-01F, /*[ 52] */ (SGLfloat) 4.198434432e-01F,        /*[ 52] */
    (SGLfloat) 5.938162585e-01F, /*[ 53] */ (SGLfloat) 4.210048514e-01F,        /*[ 53] */
    (SGLfloat) 5.921871238e-01F, /*[ 54] */ (SGLfloat) 4.221630644e-01F,        /*[ 54] */
    (SGLfloat) 5.905713245e-01F, /*[ 55] */ (SGLfloat) 4.233181084e-01F,        /*[ 55] */
    (SGLfloat) 5.889686798e-01F, /*[ 56] */ (SGLfloat) 4.244700093e-01F,        /*[ 56] */
    (SGLfloat) 5.873790120e-01F, /*[ 57] */ (SGLfloat) 4.256187927e-01F,        /*[ 57] */
    (SGLfloat) 5.858021471e-01F, /*[ 58] */ (SGLfloat) 4.267644836e-01F,        /*[ 58] */
    (SGLfloat) 5.842379140e-01F, /*[ 59] */ (SGLfloat) 4.279071070e-01F,        /*[ 59] */
    (SGLfloat) 5.826861450e-01F, /*[ 60] */ (SGLfloat) 4.290466873e-01F,        /*[ 60] */
    (SGLfloat) 5.811466756e-01F, /*[ 61] */ (SGLfloat) 4.301832487e-01F,        /*[ 61] */
    (SGLfloat) 5.796193439e-01F, /*[ 62] */ (SGLfloat) 4.313168152e-01F,        /*[ 62] */
    (SGLfloat) 5.781039915e-01F, /*[ 63] */ (SGLfloat) 4.324474102e-01F,        /*[ 63] */
    (SGLfloat) 5.766004624e-01F, /*[ 64] */ (SGLfloat) 4.335750570e-01F,        /*[ 64] */
    (SGLfloat) 5.751086037e-01F, /*[ 65] */ (SGLfloat) 4.346997785e-01F,        /*[ 65] */
    (SGLfloat) 5.736282654e-01F, /*[ 66] */ (SGLfloat) 4.358215975e-01F,        /*[ 66] */
    (SGLfloat) 5.721592997e-01F, /*[ 67] */ (SGLfloat) 4.369405362e-01F,        /*[ 67] */
    (SGLfloat) 5.707015619e-01F, /*[ 68] */ (SGLfloat) 4.380566167e-01F,        /*[ 68] */
    (SGLfloat) 5.692549096e-01F, /*[ 69] */ (SGLfloat) 4.391698608e-01F,        /*[ 69] */
    (SGLfloat) 5.678192031e-01F, /*[ 70] */ (SGLfloat) 4.402802900e-01F,        /*[ 70] */
    (SGLfloat) 5.663943050e-01F, /*[ 71] */ (SGLfloat) 4.413879257e-01F,        /*[ 71] */
    (SGLfloat) 5.649800804e-01F, /*[ 72] */ (SGLfloat) 4.424927886e-01F,        /*[ 72] */
    (SGLfloat) 5.635763967e-01F, /*[ 73] */ (SGLfloat) 4.435948997e-01F,        /*[ 73] */
    (SGLfloat) 5.621831236e-01F, /*[ 74] */ (SGLfloat) 4.446942793e-01F,        /*[ 74] */
    (SGLfloat) 5.608001330e-01F, /*[ 75] */ (SGLfloat) 4.457909476e-01F,        /*[ 75] */
    (SGLfloat) 5.594272991e-01F, /*[ 76] */ (SGLfloat) 4.468849246e-01F,        /*[ 76] */
    (SGLfloat) 5.580644981e-01F, /*[ 77] */ (SGLfloat) 4.479762301e-01F,        /*[ 77] */
    (SGLfloat) 5.567116085e-01F, /*[ 78] */ (SGLfloat) 4.490648834e-01F,        /*[ 78] */
    (SGLfloat) 5.553685107e-01F, /*[ 79] */ (SGLfloat) 4.501509039e-01F,        /*[ 79] */
    (SGLfloat) 5.540350871e-01F, /*[ 80] */ (SGLfloat) 4.512343106e-01F,        /*[ 80] */
    (SGLfloat) 5.527112222e-01F, /*[ 81] */ (SGLfloat) 4.523151222e-01F,        /*[ 81] */
    (SGLfloat) 5.513968023e-01F, /*[ 82] */ (SGLfloat) 4.533933572e-01F,        /*[ 82] */
    (SGLfloat) 5.500917156e-01F, /*[ 83] */ (SGLfloat) 4.544690342e-01F,        /*[ 83] */
    (SGLfloat) 5.487958522e-01F, /*[ 84] */ (SGLfloat) 4.555421710e-01F,        /*[ 84] */
    (SGLfloat) 5.475091040e-01F, /*[ 85] */ (SGLfloat) 4.566127858e-01F,        /*[ 85] */
    (SGLfloat) 5.462313646e-01F, /*[ 86] */ (SGLfloat) 4.576808961e-01F,        /*[ 86] */
    (SGLfloat) 5.449625293e-01F, /*[ 87] */ (SGLfloat) 4.587465194e-01F,        /*[ 87] */
    (SGLfloat) 5.437024953e-01F, /*[ 88] */ (SGLfloat) 4.598096731e-01F,        /*[ 88] */
    (SGLfloat) 5.424511612e-01F, /*[ 89] */ (SGLfloat) 4.608703743e-01F,        /*[ 89] */
    (SGLfloat) 5.412084274e-01F, /*[ 90] */ (SGLfloat) 4.619286398e-01F,        /*[ 90] */
    (SGLfloat) 5.399741959e-01F, /*[ 91] */ (SGLfloat) 4.629844863e-01F,        /*[ 91] */
    (SGLfloat) 5.387483700e-01F, /*[ 92] */ (SGLfloat) 4.640379304e-01F,        /*[ 92] */
    (SGLfloat) 5.375308549e-01F, /*[ 93] */ (SGLfloat) 4.650889884e-01F,        /*[ 93] */
    (SGLfloat) 5.363215571e-01F, /*[ 94] */ (SGLfloat) 4.661376763e-01F,        /*[ 94] */
    (SGLfloat) 5.351203844e-01F, /*[ 95] */ (SGLfloat) 4.671840103e-01F,        /*[ 95] */
    (SGLfloat) 5.339272465e-01F, /*[ 96] */ (SGLfloat) 4.682280060e-01F,        /*[ 96] */
    (SGLfloat) 5.327420540e-01F, /*[ 97] */ (SGLfloat) 4.692696791e-01F,        /*[ 97] */
    (SGLfloat) 5.315647191e-01F, /*[ 98] */ (SGLfloat) 4.703090450e-01F,        /*[ 98] */
    (SGLfloat) 5.303951556e-01F, /*[ 99] */ (SGLfloat) 4.713461190e-01F,        /*[ 99] */
    (SGLfloat) 5.292332781e-01F, /*[100] */ (SGLfloat) 4.723809161e-01F,        /*[100] */
    (SGLfloat) 5.280790029e-01F, /*[101] */ (SGLfloat) 4.734134513e-01F,        /*[101] */
    (SGLfloat) 5.269322475e-01F, /*[102] */ (SGLfloat) 4.744437394e-01F,        /*[102] */
    (SGLfloat) 5.257929305e-01F, /*[103] */ (SGLfloat) 4.754717950e-01F,        /*[103] */
    (SGLfloat) 5.246609719e-01F, /*[104] */ (SGLfloat) 4.764976324e-01F,        /*[104] */
    (SGLfloat) 5.235362929e-01F, /*[105] */ (SGLfloat) 4.775212661e-01F,        /*[105] */
    (SGLfloat) 5.224188156e-01F, /*[106] */ (SGLfloat) 4.785427101e-01F,        /*[106] */
    (SGLfloat) 5.213084637e-01F, /*[107] */ (SGLfloat) 4.795619785e-01F,        /*[107] */
    (SGLfloat) 5.202051617e-01F, /*[108] */ (SGLfloat) 4.805790851e-01F,        /*[108] */
    (SGLfloat) 5.191088352e-01F, /*[109] */ (SGLfloat) 4.815940435e-01F,        /*[109] */
    (SGLfloat) 5.180194112e-01F, /*[110] */ (SGLfloat) 4.826068675e-01F,        /*[110] */
    (SGLfloat) 5.169368174e-01F, /*[111] */ (SGLfloat) 4.836175702e-01F,        /*[111] */
    (SGLfloat) 5.158609829e-01F, /*[112] */ (SGLfloat) 4.846261651e-01F,        /*[112] */
    (SGLfloat) 5.147918375e-01F, /*[113] */ (SGLfloat) 4.856326652e-01F,        /*[113] */
    (SGLfloat) 5.137293123e-01F, /*[114] */ (SGLfloat) 4.866370836e-01F,        /*[114] */
    (SGLfloat) 5.126733391e-01F, /*[115] */ (SGLfloat) 4.876394332e-01F,        /*[115] */
    (SGLfloat) 5.116238510e-01F, /*[116] */ (SGLfloat) 4.886397265e-01F,        /*[116] */
    (SGLfloat) 5.105807818e-01F, /*[117] */ (SGLfloat) 4.896379764e-01F,        /*[117] */
    (SGLfloat) 5.095440663e-01F, /*[118] */ (SGLfloat) 4.906341951e-01F,        /*[118] */
    (SGLfloat) 5.085136404e-01F, /*[119] */ (SGLfloat) 4.916283952e-01F,        /*[119] */
    (SGLfloat) 5.074894406e-01F, /*[120] */ (SGLfloat) 4.926205887e-01F,        /*[120] */
    (SGLfloat) 5.064714045e-01F, /*[121] */ (SGLfloat) 4.936107878e-01F,        /*[121] */
    (SGLfloat) 5.054594706e-01F, /*[122] */ (SGLfloat) 4.945990045e-01F,        /*[122] */
    (SGLfloat) 5.044535781e-01F, /*[123] */ (SGLfloat) 4.955852507e-01F,        /*[123] */
    (SGLfloat) 5.034536672e-01F, /*[124] */ (SGLfloat) 4.965695380e-01F,        /*[124] */
    (SGLfloat) 5.024596788e-01F, /*[125] */ (SGLfloat) 4.975518781e-01F,        /*[125] */
    (SGLfloat) 5.014715546e-01F, /*[126] */ (SGLfloat) 4.985322826e-01F,        /*[126] */
    (SGLfloat) 5.004892373e-01F, /*[127] */ (SGLfloat) 4.995107627e-01F,        /*[127] */
    (SGLfloat) 4.990272337e-01F, /*[128] */ (SGLfloat) 5.009727663e-01F,        /*[128] */
    (SGLfloat) 4.970967391e-01F, /*[129] */ (SGLfloat) 5.029183429e-01F,        /*[129] */
    (SGLfloat) 4.951884771e-01F, /*[130] */ (SGLfloat) 5.048564214e-01F,        /*[130] */
    (SGLfloat) 4.933020243e-01F, /*[131] */ (SGLfloat) 5.067870881e-01F,        /*[131] */
    (SGLfloat) 4.914369682e-01F, /*[132] */ (SGLfloat) 5.087104271e-01F,        /*[132] */
    (SGLfloat) 4.895929076e-01F, /*[133] */ (SGLfloat) 5.106265214e-01F,        /*[133] */
    (SGLfloat) 4.877694513e-01F, /*[134] */ (SGLfloat) 5.125354522e-01F,        /*[134] */
    (SGLfloat) 4.859662185e-01F, /*[135] */ (SGLfloat) 5.144372992e-01F,        /*[135] */
    (SGLfloat) 4.841828382e-01F, /*[136] */ (SGLfloat) 5.163321408e-01F,        /*[136] */
    (SGLfloat) 4.824189487e-01F, /*[137] */ (SGLfloat) 5.182200538e-01F,        /*[137] */
    (SGLfloat) 4.806741976e-01F, /*[138] */ (SGLfloat) 5.201011136e-01F,        /*[138] */
    (SGLfloat) 4.789482412e-01F, /*[139] */ (SGLfloat) 5.219753944e-01F,        /*[139] */
    (SGLfloat) 4.772407445e-01F, /*[140] */ (SGLfloat) 5.238429688e-01F,        /*[140] */
    (SGLfloat) 4.755513808e-01F, /*[141] */ (SGLfloat) 5.257039085e-01F,        /*[141] */
    (SGLfloat) 4.738798315e-01F, /*[142] */ (SGLfloat) 5.275582836e-01F,        /*[142] */
    (SGLfloat) 4.722257855e-01F, /*[143] */ (SGLfloat) 5.294061631e-01F,        /*[143] */
    (SGLfloat) 4.705889396e-01F, /*[144] */ (SGLfloat) 5.312476147e-01F,        /*[144] */
    (SGLfloat) 4.689689977e-01F, /*[145] */ (SGLfloat) 5.330827052e-01F,        /*[145] */
    (SGLfloat) 4.673656708e-01F, /*[146] */ (SGLfloat) 5.349114999e-01F,        /*[146] */
    (SGLfloat) 4.657786768e-01F, /*[147] */ (SGLfloat) 5.367340633e-01F,        /*[147] */
    (SGLfloat) 4.642077404e-01F, /*[148] */ (SGLfloat) 5.385504586e-01F,        /*[148] */
    (SGLfloat) 4.626525925e-01F, /*[149] */ (SGLfloat) 5.403607479e-01F,        /*[149] */
    (SGLfloat) 4.611129704e-01F, /*[150] */ (SGLfloat) 5.421649926e-01F,        /*[150] */
    (SGLfloat) 4.595886174e-01F, /*[151] */ (SGLfloat) 5.439632527e-01F,        /*[151] */
    (SGLfloat) 4.580792830e-01F, /*[152] */ (SGLfloat) 5.457555873e-01F,        /*[152] */
    (SGLfloat) 4.565847220e-01F, /*[153] */ (SGLfloat) 5.475420548e-01F,        /*[153] */
    (SGLfloat) 4.551046950e-01F, /*[154] */ (SGLfloat) 5.493227123e-01F,        /*[154] */
    (SGLfloat) 4.536389680e-01F, /*[155] */ (SGLfloat) 5.510976161e-01F,        /*[155] */
    (SGLfloat) 4.521873121e-01F, /*[156] */ (SGLfloat) 5.528668217e-01F,        /*[156] */
    (SGLfloat) 4.507495037e-01F, /*[157] */ (SGLfloat) 5.546303835e-01F,        /*[157] */
    (SGLfloat) 4.493253240e-01F, /*[158] */ (SGLfloat) 5.563883554e-01F,        /*[158] */
    (SGLfloat) 4.479145590e-01F, /*[159] */ (SGLfloat) 5.581407900e-01F,        /*[159] */
    (SGLfloat) 4.465169994e-01F, /*[160] */ (SGLfloat) 5.598877395e-01F,        /*[160] */
    (SGLfloat) 4.451324406e-01F, /*[161] */ (SGLfloat) 5.616292549e-01F,        /*[161] */
    (SGLfloat) 4.437606821e-01F, /*[162] */ (SGLfloat) 5.633653867e-01F,        /*[162] */
    (SGLfloat) 4.424015281e-01F, /*[163] */ (SGLfloat) 5.650961844e-01F,        /*[163] */
    (SGLfloat) 4.410547865e-01F, /*[164] */ (SGLfloat) 5.668216971e-01F,        /*[164] */
    (SGLfloat) 4.397202696e-01F, /*[165] */ (SGLfloat) 5.685419727e-01F,        /*[165] */
    (SGLfloat) 4.383977937e-01F, /*[166] */ (SGLfloat) 5.702570587e-01F,        /*[166] */
    (SGLfloat) 4.370871786e-01F, /*[167] */ (SGLfloat) 5.719670018e-01F,        /*[167] */
    (SGLfloat) 4.357882482e-01F, /*[168] */ (SGLfloat) 5.736718480e-01F,        /*[168] */
    (SGLfloat) 4.345008299e-01F, /*[169] */ (SGLfloat) 5.753716425e-01F,        /*[169] */
    (SGLfloat) 4.332247546e-01F, /*[170] */ (SGLfloat) 5.770664300e-01F,        /*[170] */
    (SGLfloat) 4.319598567e-01F, /*[171] */ (SGLfloat) 5.787562545e-01F,        /*[171] */
    (SGLfloat) 4.307059740e-01F, /*[172] */ (SGLfloat) 5.804411594e-01F,        /*[172] */
    (SGLfloat) 4.294629475e-01F, /*[173] */ (SGLfloat) 5.821211874e-01F,        /*[173] */
    (SGLfloat) 4.282306215e-01F, /*[174] */ (SGLfloat) 5.837963805e-01F,        /*[174] */
    (SGLfloat) 4.270088434e-01F, /*[175] */ (SGLfloat) 5.854667803e-01F,        /*[175] */
    (SGLfloat) 4.257974635e-01F, /*[176] */ (SGLfloat) 5.871324277e-01F,        /*[176] */
    (SGLfloat) 4.245963351e-01F, /*[177] */ (SGLfloat) 5.887933630e-01F,        /*[177] */
    (SGLfloat) 4.234053145e-01F, /*[178] */ (SGLfloat) 5.904496260e-01F,        /*[178] */
    (SGLfloat) 4.222242607e-01F, /*[179] */ (SGLfloat) 5.921012560e-01F,        /*[179] */
    (SGLfloat) 4.210530354e-01F, /*[180] */ (SGLfloat) 5.937482915e-01F,        /*[180] */
    (SGLfloat) 4.198915032e-01F, /*[181] */ (SGLfloat) 5.953907707e-01F,        /*[181] */
    (SGLfloat) 4.187395310e-01F, /*[182] */ (SGLfloat) 5.970287312e-01F,        /*[182] */
    (SGLfloat) 4.175969883e-01F, /*[183] */ (SGLfloat) 5.986622101e-01F,        /*[183] */
    (SGLfloat) 4.164637474e-01F, /*[184] */ (SGLfloat) 6.002912440e-01F,        /*[184] */
    (SGLfloat) 4.153396825e-01F, /*[185] */ (SGLfloat) 6.019158690e-01F,        /*[185] */
    (SGLfloat) 4.142246706e-01F, /*[186] */ (SGLfloat) 6.035361206e-01F,        /*[186] */
    (SGLfloat) 4.131185908e-01F, /*[187] */ (SGLfloat) 6.051520341e-01F,        /*[187] */
    (SGLfloat) 4.120213245e-01F, /*[188] */ (SGLfloat) 6.067636440e-01F,        /*[188] */
    (SGLfloat) 4.109327552e-01F, /*[189] */ (SGLfloat) 6.083709847e-01F,        /*[189] */
    (SGLfloat) 4.098527686e-01F, /*[190] */ (SGLfloat) 6.099740897e-01F,        /*[190] */
    (SGLfloat) 4.087812526e-01F, /*[191] */ (SGLfloat) 6.115729925e-01F,        /*[191] */
    (SGLfloat) 4.077180970e-01F, /*[192] */ (SGLfloat) 6.131677259e-01F,        /*[192] */
    (SGLfloat) 4.066631936e-01F, /*[193] */ (SGLfloat) 6.147583224e-01F,        /*[193] */
    (SGLfloat) 4.056164363e-01F, /*[194] */ (SGLfloat) 6.163448139e-01F,        /*[194] */
    (SGLfloat) 4.045777207e-01F, /*[195] */ (SGLfloat) 6.179272322e-01F,        /*[195] */
    (SGLfloat) 4.035469444e-01F, /*[196] */ (SGLfloat) 6.195056084e-01F,        /*[196] */
    (SGLfloat) 4.025240068e-01F, /*[197] */ (SGLfloat) 6.210799733e-01F,        /*[197] */
    (SGLfloat) 4.015088090e-01F, /*[198] */ (SGLfloat) 6.226503574e-01F,        /*[198] */
    (SGLfloat) 4.005012539e-01F, /*[199] */ (SGLfloat) 6.242167908e-01F,        /*[199] */
    (SGLfloat) 3.995012461e-01F, /*[200] */ (SGLfloat) 6.257793030e-01F,        /*[200] */
    (SGLfloat) 3.985086918e-01F, /*[201] */ (SGLfloat) 6.273379234e-01F,        /*[201] */
    (SGLfloat) 3.975234989e-01F, /*[202] */ (SGLfloat) 6.288926809e-01F,        /*[202] */
    (SGLfloat) 3.965455769e-01F, /*[203] */ (SGLfloat) 6.304436041e-01F,        /*[203] */
    (SGLfloat) 3.955748367e-01F, /*[204] */ (SGLfloat) 6.319907212e-01F,        /*[204] */
    (SGLfloat) 3.946111909e-01F, /*[205] */ (SGLfloat) 6.335340602e-01F,        /*[205] */
    (SGLfloat) 3.936545535e-01F, /*[206] */ (SGLfloat) 6.350736485e-01F,        /*[206] */
    (SGLfloat) 3.927048400e-01F, /*[207] */ (SGLfloat) 6.366095135e-01F,        /*[207] */
    (SGLfloat) 3.917619671e-01F, /*[208] */ (SGLfloat) 6.381416818e-01F,        /*[208] */
    (SGLfloat) 3.908258533e-01F, /*[209] */ (SGLfloat) 6.396701802e-01F,        /*[209] */
    (SGLfloat) 3.898964180e-01F, /*[210] */ (SGLfloat) 6.411950349e-01F,        /*[210] */
    (SGLfloat) 3.889735824e-01F, /*[211] */ (SGLfloat) 6.427162718e-01F,        /*[211] */
    (SGLfloat) 3.880572686e-01F, /*[212] */ (SGLfloat) 6.442339165e-01F,        /*[212] */
    (SGLfloat) 3.871474002e-01F, /*[213] */ (SGLfloat) 6.457479944e-01F,        /*[213] */
    (SGLfloat) 3.862439020e-01F, /*[214] */ (SGLfloat) 6.472585304e-01F,        /*[214] */
    (SGLfloat) 3.853467000e-01F, /*[215] */ (SGLfloat) 6.487655494e-01F,        /*[215] */
    (SGLfloat) 3.844557214e-01F, /*[216] */ (SGLfloat) 6.502690758e-01F,        /*[216] */
    (SGLfloat) 3.835708946e-01F, /*[217] */ (SGLfloat) 6.517691338e-01F,        /*[217] */
    (SGLfloat) 3.826921491e-01F, /*[218] */ (SGLfloat) 6.532657472e-01F,        /*[218] */
    (SGLfloat) 3.818194156e-01F, /*[219] */ (SGLfloat) 6.547589397e-01F,        /*[219] */
    (SGLfloat) 3.809526258e-01F, /*[220] */ (SGLfloat) 6.562487346e-01F,        /*[220] */
    (SGLfloat) 3.800917126e-01F, /*[221] */ (SGLfloat) 6.577351550e-01F,        /*[221] */
    (SGLfloat) 3.792366099e-01F, /*[222] */ (SGLfloat) 6.592182238e-01F,        /*[222] */
    (SGLfloat) 3.783872526e-01F, /*[223] */ (SGLfloat) 6.606979635e-01F,        /*[223] */
    (SGLfloat) 3.775435766e-01F, /*[224] */ (SGLfloat) 6.621743964e-01F,        /*[224] */
    (SGLfloat) 3.767055190e-01F, /*[225] */ (SGLfloat) 6.636475446e-01F,        /*[225] */
    (SGLfloat) 3.758730175e-01F, /*[226] */ (SGLfloat) 6.651174300e-01F,        /*[226] */
    (SGLfloat) 3.750460112e-01F, /*[227] */ (SGLfloat) 6.665840741e-01F,        /*[227] */
    (SGLfloat) 3.742244398e-01F, /*[228] */ (SGLfloat) 6.680474982e-01F,        /*[228] */
    (SGLfloat) 3.734082440e-01F, /*[229] */ (SGLfloat) 6.695077235e-01F,        /*[229] */
    (SGLfloat) 3.725973654e-01F, /*[230] */ (SGLfloat) 6.709647709e-01F,        /*[230] */
    (SGLfloat) 3.717917467e-01F, /*[231] */ (SGLfloat) 6.724186610e-01F,        /*[231] */
    (SGLfloat) 3.709913311e-01F, /*[232] */ (SGLfloat) 6.738694142e-01F,        /*[232] */
    (SGLfloat) 3.701960629e-01F, /*[233] */ (SGLfloat) 6.753170508e-01F,        /*[233] */
    (SGLfloat) 3.694058872e-01F, /*[234] */ (SGLfloat) 6.767615908e-01F,        /*[234] */
    (SGLfloat) 3.686207498e-01F, /*[235] */ (SGLfloat) 6.782030540e-01F,        /*[235] */
    (SGLfloat) 3.678405974e-01F, /*[236] */ (SGLfloat) 6.796414599e-01F,        /*[236] */
    (SGLfloat) 3.670653776e-01F, /*[237] */ (SGLfloat) 6.810768279e-01F,        /*[237] */
    (SGLfloat) 3.662950384e-01F, /*[238] */ (SGLfloat) 6.825091772e-01F,        /*[238] */
    (SGLfloat) 3.655295290e-01F, /*[239] */ (SGLfloat) 6.839385268e-01F,        /*[239] */
    (SGLfloat) 3.647687991e-01F, /*[240] */ (SGLfloat) 6.853648954e-01F,        /*[240] */
    (SGLfloat) 3.640127992e-01F, /*[241] */ (SGLfloat) 6.867883015e-01F,        /*[241] */
    (SGLfloat) 3.632614804e-01F, /*[242] */ (SGLfloat) 6.882087636e-01F,        /*[242] */
    (SGLfloat) 3.625147946e-01F, /*[243] */ (SGLfloat) 6.896262999e-01F,        /*[243] */
    (SGLfloat) 3.617726944e-01F, /*[244] */ (SGLfloat) 6.910409284e-01F,        /*[244] */
    (SGLfloat) 3.610351331e-01F, /*[245] */ (SGLfloat) 6.924526668e-01F,        /*[245] */
    (SGLfloat) 3.603020646e-01F, /*[246] */ (SGLfloat) 6.938615329e-01F,        /*[246] */
    (SGLfloat) 3.595734434e-01F, /*[247] */ (SGLfloat) 6.952675441e-01F,        /*[247] */
    (SGLfloat) 3.588492248e-01F, /*[248] */ (SGLfloat) 6.966707176e-01F,        /*[248] */
    (SGLfloat) 3.581293646e-01F, /*[249] */ (SGLfloat) 6.980710707e-01F,        /*[249] */
    (SGLfloat) 3.574138193e-01F, /*[250] */ (SGLfloat) 6.994686202e-01F,        /*[250] */
    (SGLfloat) 3.567025459e-01F, /*[251] */ (SGLfloat) 7.008633828e-01F,        /*[251] */
    (SGLfloat) 3.559955021e-01F, /*[252] */ (SGLfloat) 7.022553753e-01F,        /*[252] */
    (SGLfloat) 3.552926461e-01F, /*[253] */ (SGLfloat) 7.036446140e-01F,        /*[253] */
    (SGLfloat) 3.545939368e-01F, /*[254] */ (SGLfloat) 7.050311153e-01F,        /*[254] */
    (SGLfloat) 3.538993336e-01F, /*[255] */ (SGLfloat) 7.064148952e-01F /*[255] */
};

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M18.4, 6, "Rule M18.4 is not applied here as algorithm uses the 32 bits representation of float and long to compute the new mantissa through the union.")
#endif
/* local types definitions */
typedef union {
    SGLfloat f;
    SGLulong l;
} sgl_flt_bin;

/******************************************************************************
 **                           Module Body
 *****************************************************************************/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_sqrtf
  DESCRIPTION:
    Function shall return a square root for a positive value. 
	{
    This function computes the nonnegative square root of par_f_val. 
    It shall be equivalent to sqrtf(par_f_val) with a precision of 2.0e-4. 
    If par_f_val is less or equal to 0.0 the function shall return 0.0. 
    This algorithm relies on the IEEE 32-bit floating-point 
    format. 
	}
    A float is coded as (the values are for example): 
    {
    0				01110001			01111000001111100001010
    sign(1 bit)			exponent(8 bits)		mantissa(23 bits)
    }
    This format defines a float as m * 2 ^ e. The square root of 
    such a float is (m * 2 ^ e) ^ 1/2. 
    The computation of the new mantissa shall be an approximation performed through an 
    interpolation table	stc_tf_tabforsqrt with 2 * 256 entries. Each time 
    a square root is computed, two consecutive entries in the table shall be 
    considered to compute the resulted mantissa. Index i shall be computed as 
    (m >> 16) * 2. The resulted mantissa shall be then (stc_tf_tabforsqrt[i] * m) + stc_tf_tabforsqrt[i+1]. 
    The new exponent shall simply be approximated to e/2. 
  SCOPE: global
  PARAMETERS:
    par_f_val -> The input float value            
  RETURN:
    SGLfloat -> Square root for a positive value  
---------------------------------------------------------------------+*/
SGLfloat mth_sqrtf(SGLfloat par_f_val)
{
    /* internal data */
    SGLulong loc_ul_i;
    SGLulong loc_ul_n;
    SGLulong loc_ul_m;
    sgl_flt_bin loc_r_x;
    sgl_flt_bin loc_r_y;

    if (par_f_val > (SGLfloat) 0.0F) {
        /* start of code */
        loc_r_x.f = (SGLfloat) par_f_val;
        loc_ul_n = ((loc_r_x.l & 0x7F800000U) >> 23U) - 126U;
        loc_ul_m = loc_r_x.l & 0x80FFFFFFU;
        /*
         * * 0.5 <= loc_r_x < 2.0
         */
        loc_r_x.l = loc_ul_m | 0x3F000000U;
        /*
         * * 0  <= loc_ul_i < 256
         */
        loc_ul_i = (loc_ul_m >> 16U) * 2U;
        /* Approximate the new matissa through  the interpolation table */
        loc_r_y.f = (SGLfloat) ((stc_tf_tabforsqrt[loc_ul_i] * loc_r_x.f) + stc_tf_tabforsqrt[loc_ul_i + 1U]);
        /* Add the exponent, loc_ul_n >> 1U divides the exponent by 2 */
        loc_r_y.l = loc_r_y.l + ((loc_ul_n >> 1U) << 23U);
    }
    else {
        /*
         * * Set the return value
         */
        loc_r_y.f = 0.0F;
    }

    return loc_r_y.f;

}

/* End of File ***************************************************************/
