
hw_phy.h,4646
#define __HW_PHY_H__30,981
#define	CNTRL_A_REG	62,1834
#define	CNTRL_B_REG	63,1864
#define	INTR_MASK_REG	64,1894
#define	XMIT_VECTOR_REG	65,1925
#define	VECTOR_LENGTH_REG	66,1958
#define	LE_THRESHOLD_REG	67,1992
#define	C_MIN_REG	68,2025
#define	A_MAX_REG	69,2053
#define	TL_MIN_REG	70,2105
#define	LS_MAX_REG	71,2134
#define	TB_MIN_REG	72,2188
#define	T_OUT_REG	73,2217
#define	LC_SHORT_REG	74,2245
#define	T_SCRUB_REG	75,2275
#define	NS_MAX_REG	76,2305
#define	TPC_LOAD_VALUE_REG	77,2334
#define	TNE_LOAD_VALUE_REG	78,2369
#define	STATUS_A_REG	79,2404
#define	STATUS_B_REG	80,2434
#define	TPC_REG	81,2464
#define	TNE_REG	82,2491
#define	CLK_DIV_REG	83,2518
#define	BIST_SIGNATURE_REG	84,2548
#define	RCV_VECTOR_REG	85,2583
#define	INTR_EVENT_REG	86,2615
#define	VIOL_SYM_CTR_REG	87,2647
#define	MIN_IDLE_CTR_REG	88,2680
#define	LINK_ERR_CTR_REG	89,2713
#define FOTOFF_ASSERT_REG	90,2746
#define FOTOFF_DEASSERT_REG	91,2780
#define	NOISE_TIMER_MASK	96,2856
#define	TNE_16BIT_MASK	97,2892
#define	TPC_16BIT_MASK	98,2927
#define	REQ_SCRUB_MASK	99,2962
#define	VSYM_CTR_INTRS_MASK	100,2997
#define	MINI_CTR_INTRS_MASK	101,3035
#define	FCG_LOOP_CNTRL_MASK	102,3073
#define	CD_LOOP_CNTRL_MASK	103,3130
#define	FOT_OFF_MASK	104,3181
#define	EB_LOC_LOOP_MASK	105,3213
#define	ELM_LOC_LOOP_MASK	106,3248
#define	LM_LOC_LOOP_MASK	107,3303
#define	SC_BYPASS_MASK	108,3352
#define	SC_REM_LOOP_MASK	109,3386
#define	RF_DISABLE_MASK	110,3421
#define	RUN_BIST_MASK	111,3456
#define	CONFIG_CNTRL_MASK	116,3524
#define	MATCH_LS_MASK	117,3587
#define	MAINT_LS_MASK	118,3649
#define	CLASS_S_MASK	119,3707
#define	PC_LOOP_MASK	120,3764
#define	PC_JOIN_MASK	121,3821
#define	LONG_MASK	122,3878
#define	PC_MAINT_MASK	123,3933
#define	PCM_CNTRL_MASK	124,3991
#define	SIGNAL_DETECT_MASK	129,4086
#define	PREV_LINE_ST_MASK	130,4124
#define	LINE_ST_MASK	131,4160
#define	LSM_STATE_MASK	132,4192
#define	UNKN_LINE_ST_MASK	133,4226
#define	SYM_PR_CTR_MASK	134,4262
#define	RF_STATE_MASK	139,4331
#define	PCI_STATE_MASK	140,4365
#define	PCI_SCRUB_MASK	141,4400
#define	PCM_STATE_MASK	142,4435
#define	PCM_SIGNALING_MASK	143,4471
#define	LSF_MASK	144,4508
#define	RCF_MASK	145,4537
#define	TCF_MASK	146,4566
#define	BREAK_REASON_MASK	147,4595
#define	INTR_NP_ERR	152,4660
#define	INTR_LSD	153,4690
#define	INTR_LE_CTR	154,4717
#define	INTR_MINI_CTR	155,4747
#define	INTR_VSYM_CTR	156,4778
#define	INTR_PHYINV	157,4809
#define	INTR_EBUF_ERR	158,4839
#define	INTR_TNE_EXPIRED	159,4870
#define	INTR_TPC_EXPIRED	160,4903
#define	INTR_PCM_ENABLED	161,4936
#define	INTR_PCM_BREAK	162,4969
#define	INTR_SELF_TEST	163,5001
#define	INTR_TRACE_PROP	164,5033
#define	INTR_PCM_CODE	165,5066
#define	INTR_LS_MATCH	166,5097
#define	INTR_PARITY_ERR	167,5128
#define	MATCH_LS_ANY	174,5263
#define	MATCH_LS_QLS	175,5288
#define	MATCH_LS_MLS	176,5321
#define	MATCH_LS_HLS	177,5354
#define	MATCH_LS_ILS	178,5387
#define	MAINT_LS_ILS	180,5421
#define	MAINT_LS_HLS	181,5453
#define	MAINT_LS_MLS	182,5485
#define	MAINT_LS_QLS	183,5517
#define	MAINT_LS_PDR	184,5578
#define	PC_LOOP_NO_LCT	186,5611
#define	PC_LOOP_PDR_PDU	187,5638
#define	PC_LOOP_IDLE	188,5673
#define	PC_LOOP_PDR_LOOPBACK	189,5705
#define	PCM_CNTRL_NONE	191,5744
#define	PCM_CNTRL_BREAK	192,5771
#define	PCM_CNTRL_TRACE	193,5799
#define	PCM_CNTRL_OFF	194,5827
#define	PREV_LINE_ST_QLS	196,5854
#define	PREV_LINE_ST_MLS	197,5882
#define	PREV_LINE_ST_HLS	198,5917
#define	PREV_LINE_ST_ILS	199,5952
#define	LINE_ST_NLS	201,6009
#define	LINE_ST_ALS	202,6034
#define	LINE_ST_ILS4	203,6066
#define	LINE_ST_QLS	204,6098
#define	LINE_ST_MLS	205,6130
#define	LINE_ST_HLS	206,6162
#define	LINE_ST_ILS16	207,6194
#define	RF_STATE_REPEAT	209,6228
#define	RF_STATE_IDLE	210,6256
#define	RF_STATE_HALT1	211,6290
#define	RF_STATE_HALT2	212,6325
#define	PCI_STATE_REMOVED	214,6361
#define	PCI_STATE_INSERT_SCRUB	215,6390
#define	PCI_STATE_REMOVE_SCRUB	216,6431
#define	PCI_STATE_INSERTED	217,6472
#define	PCM_STATE_OFF	219,6511
#define	PCM_STATE_BREAK	220,6549
#define	PCM_STATE_TRACE	221,6591
#define	PCM_STATE_CONNECT	222,6633
#define	PCM_STATE_NEXT	223,6678
#define	PCM_STATE_SIGNAL	224,6718
#define	PCM_STATE_JOIN	225,6761
#define	PCM_STATE_VERIFY	226,6801
#define	PCM_STATE_ACTIVE	227,6844
#define	PCM_STATE_MAINT	228,6887
#define	BREAK_NONE	230,6930
#define	BREAK_PC_START	231,6954
#define	BREAK_T_OUT	232,6981
#define	BREAK_NS_MAX	233,7006
#define	BREAK_QLS	234,7031
#define	BREAK_ILS	235,7054
#define	BREAK_HLS	236,7077
#define	TPC_VALUE(TPC_VALUE243,7212
#define	TNE_VALUE(TNE_VALUE244,7270
