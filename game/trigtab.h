#define NSINEVALUES		256
#define NASINEVALUES	256
#define NATANVALUES		256


const S16 sintab[NSINEVALUES+1] = {
	0,101,201,302,402,503,603,704,
	804,904,1005,1105,1205,1306,1406,1506,
	1606,1706,1806,1906,2006,2105,2205,2305,
	2404,2503,2603,2702,2801,2900,2999,3098,
	3196,3295,3393,3492,3590,3688,3786,3883,
	3981,4078,4176,4273,4370,4467,4563,4660,
	4756,4852,4948,5044,5139,5235,5330,5425,
	5520,5614,5708,5803,5897,5990,6084,6177,
	6270,6363,6455,6547,6639,6731,6823,6914,
	7005,7096,7186,7276,7366,7456,7545,7635,
	7723,7812,7900,7988,8076,8163,8250,8337,
	8423,8509,8595,8680,8765,8850,8935,9019,
	9102,9186,9269,9352,9434,9516,9598,9679,
	9760,9841,9921,10001,10080,10159,10238,10316,
	10394,10471,10549,10625,10702,10778,10853,10928,
	11003,11077,11151,11224,11297,11370,11442,11514,
	11585,11656,11727,11797,11866,11935,12004,12072,
	12140,12207,12274,12340,12406,12472,12537,12601,
	12665,12729,12792,12854,12916,12978,13039,13100,
	13160,13219,13279,13337,13395,13453,13510,13567,
	13623,13678,13733,13788,13842,13896,13949,14001,
	14053,14104,14155,14206,14256,14305,14354,14402,
	14449,14497,14543,14589,14635,14680,14724,14768,
	14811,14854,14896,14937,14978,15019,15059,15098,
	15137,15175,15213,15250,15286,15322,15357,15392,
	15426,15460,15493,15525,15557,15588,15619,15649,
	15679,15707,15736,15763,15791,15817,15843,15868,
	15893,15917,15941,15964,15986,16008,16029,16049,
	16069,16088,16107,16125,16143,16160,16176,16192,
	16207,16221,16235,16248,16261,16273,16284,16295,
	16305,16315,16324,16332,16340,16347,16353,16359,
	16364,16369,16373,16376,16379,16381,16383,16384,
	16384
};


const S16 asintab[NASINEVALUES+1]={
	-16383,-15079,-14537,-14121,-13769,-13459,-13177,-12918,
    	-12676,-12449,-12233,-12028,-11831,-11642,-11460,-11283,
    	-11112,-10946,-10785,-10628,-10474,-10324,-10177,-10033,
    	-9892,-9753,-9617,-9484,-9352,-9223,-9095,-8969,
    	-8845,-8723,-8602,-8483,-8365,-8248,-8133,-8019,
    	-7906,-7795,-7684,-7574,-7466,-7358,-7252,-7146,
    	-7041,-6937,-6834,-6732,-6630,-6529,-6429,-6330,
    	-6231,-6132,-6035,-5938,-5841,-5745,-5650,-5555,
    	-5461,-5367,-5274,-5181,-5088,-4996,-4905,-4813,
    	-4723,-4632,-4542,-4452,-4363,-4274,-4185,-4097,
    	-4009,-3921,-3834,-3747,-3660,-3573,-3487,-3400,
    	-3315,-3229,-3143,-3058,-2973,-2888,-2804,-2719,
    	-2635,-2551,-2467,-2383,-2300,-2216,-2133,-2050,
    	-1967,-1884,-1801,-1719,-1636,-1554,-1471,-1389,
    	-1307,-1225,-1143,-1061,-979,-897,-815,-733,
    	-652,-570,-489,-407,-326,-244,-162,-81,    
	0,81,162,244,326,407,489,570,
	652,733,815,897,979,1061,1143,1225,
	1307,1389,1471,1554,1636,1719,1801,1884,
	1967,2050,2133,2216,2300,2383,2467,2551,
	2635,2719,2804,2888,2973,3058,3143,3229,
	3315,3400,3487,3573,3660,3747,3834,3921,
	4009,4097,4185,4274,4363,4452,4542,4632,
	4723,4813,4905,4996,5088,5181,5274,5367,
	5461,5555,5650,5745,5841,5938,6035,6132,
	6231,6330,6429,6529,6630,6732,6834,6937,
	7041,7146,7252,7358,7466,7574,7684,7795,
	7906,8019,8133,8248,8365,8483,8602,8723,
	8845,8969,9095,9223,9352,9484,9617,9753,
	9892,10033,10177,10324,10474,10628,10785,10946,
	11112,11283,11460,11642,11831,12028,12233,12449,
	12676,12918,13177,13459,13769,14121,14537,15079,
	16383
};
	


const S16 atantab[NATANVALUES+1] = {
	0,41,81,122,163,204,244,285,
	326,367,407,448,489,529,570,610,
	651,692,732,773,813,854,894,935,
	975,1015,1056,1096,1136,1177,1217,1257,
	1297,1337,1377,1417,1457,1497,1537,1577,
	1617,1656,1696,1736,1775,1815,1854,1894,
	1933,1973,2012,2051,2090,2129,2168,2207,
	2246,2285,2324,2363,2401,2440,2478,2517,
	2555,2594,2632,2670,2708,2746,2784,2822,
	2860,2897,2935,2973,3010,3047,3085,3122,
	3159,3196,3233,3270,3307,3344,3380,3417,
	3453,3490,3526,3562,3599,3635,3670,3706,
	3742,3778,3813,3849,3884,3920,3955,3990,
	4025,4060,4095,4129,4164,4199,4233,4267,
	4302,4336,4370,4404,4438,4471,4505,4539,
	4572,4605,4639,4672,4705,4738,4771,4803,
	4836,4869,4901,4933,4966,4998,5030,5062,
	5094,5125,5157,5188,5220,5251,5282,5313,
	5344,5375,5406,5437,5467,5498,5528,5559,
	5589,5619,5649,5679,5708,5738,5768,5797,
	5826,5856,5885,5914,5943,5972,6000,6029,
	6058,6086,6114,6142,6171,6199,6227,6254,
	6282,6310,6337,6365,6392,6419,6446,6473,
	6500,6527,6554,6580,6607,6633,6660,6686,
	6712,6738,6764,6790,6815,6841,6867,6892,
	6917,6943,6968,6993,7018,7043,7068,7092,
	7117,7141,7166,7190,7214,7238,7262,7286,
	7310,7334,7358,7381,7405,7428,7451,7475,
	7498,7521,7544,7566,7589,7612,7635,7657,
	7679,7702,7724,7746,7768,7790,7812,7834,
	7856,7877,7899,7920,7942,7963,7984,8005,
	8026,8047,8068,8089,8110,8131,8151,8172,
	8192
};
