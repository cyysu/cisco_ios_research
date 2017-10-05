
smds.h,3555
#define __SMDS_H__55,2075
#define SMDS_HEADER 59,2117
#define SMDS_ADDRESS 60,2140
#define SMDS_GROUP_TYPE 61,2168
#define SMDS_INDIVIDUAL_TYPE 62,2196
#define SMDS_LLC 63,2229
static inline boolean SMDS_ISGROUP 65,2249
static inline boolean SMDS_ISINDIV 69,2339
#define SMDS_TRAILER 74,2435
#define SMDSBYTES 75,2458
#define XHEL_SIZE 76,2478
#define SMDSHASH 77,2498
#define SMDS_BAD_BASIZE	79,2521
#define SMDS_BAD_XHEL	80,2548
#define SMDS_INVLD_ADDR	81,2573
#define	SMDS_BAD_BETAG	82,2600
#define SMDS_INPUT	83,2626
#define	SMDS_OUTPUT	84,2648
#define SMDS_DXI_CMDFRDSU	85,2671
#define SMDS_DXI_RSPTODSU	86,2699
#define SMDS_DXI_CMDTODSU	87,2727
#define SMDS_DXI_RSPFRDSU	88,2755
#define SMDS_DXI_BADFRAME	89,2783
#define SMDS_MAX_COUNTERS	90,2812
#define	SMDS_MINPAKLEN	92,2842
#define	SMDS_MAXPAKLEN	93,2869
#define	SMDS_BRIDGE_PAD_SIZE 95,2899
#define SMDS_BRIDGE_PAD_FDDI_EXTRA	96,2956
#define SMDS_BRIDGE_PAD_FDDI_SIZE	97,3022
#define	SMDS_BRIDGE_PAD_DATA 98,3058
#define SMDS_BRIDGE_OUI0	103,3131
#define SMDS_BRIDGE_OUI1	104,3161
#define SMDS_BRIDGE_OUI2	105,3191
#define	SMDS_BRIDGE_OUI	106,3221
#define	SMDS_BRIDGE_PID_8023FCS	107,3255
#define	SMDS_BRIDGE_PID_8024FCS	108,3317
#define	SMDS_BRIDGE_PID_8025FCS	109,3379
#define	SMDS_BRIDGE_PID_FDDIFCS	110,3441
#define	SMDS_BRIDGE_PID_8026FCS	111,3502
#define	SMDS_BRIDGE_PID_8029FCS	112,3564
#define	SMDS_BRIDGE_PID_8023	113,3626
#define	SMDS_BRIDGE_PID_8024	114,3680
#define	SMDS_BRIDGE_PID_8025	115,3734
#define	SMDS_BRIDGE_PID_FDDI	116,3788
#define	SMDS_BRIDGE_PID_8026	117,3841
#define	SMDS_BRIDGE_PID_8029	118,3895
#define	SMDS_BRIDGE_PID_BPDU	119,3949
typedef struct smds_map_ smds_map_126,4023
} smdsmaptype;smdsmaptype136,4282
struct smdsaddr_ smdsaddr_138,4298
#define	SMDS_XHLPI_HLPI	152,4626
#define	SMDS_XHLPI_PL	153,4656
#define	SMDS_XHEL_QOS	155,4684
#define	SMDS_XHEL_CIB	156,4712
#define	SMDS_XHEL_HEL	157,4740
typedef  struct _smdsheader_ _smdsheader_160,4788
} smdstype;smdstype171,5076
#define DXI_HBEAT_SIZE	176,5120
#define	DXI_LL_BITS	178,5182
#define	DXI_LM	179,5231
#define	DXI_DATA	180,5277
#define	DXI_S_BIT	181,5313
#define	DXI_CR_BIT	182,5365
#define	DXI_AE_BIT	183,5419
#define	DXI_S_CR_AE	184,5473
#define DXI_S_AE 185,5531
#define	DXI_RSPTODSU	186,5580
#define	DXI_CMDTODSU	187,5650
#define	DXI_CMDFRDSU	188,5708
#define	DXI_RSPFRDSU	189,5767
#define	DXI_UI	191,5816
#define	DXI_TEST	192,5837
#define	DXI_PF	193,5859
#define	DXI_PF_MASK	194,5895
#define DXI_MIN_BEAT	196,5941
#define DXI_MAX_BEAT	197,5996
#define DXI_NO_ACK_TMR	198,6052
typedef	struct	_dxi_	_dxi_202,6132
} dxitype;dxitype206,6212
typedef	struct	_dxismds_	_dxismds_214,6409
} dxismds;dxismds217,6472
typedef struct _dxiheartbeat_ _dxiheartbeat_222,6513
} dxiheartbeat;dxiheartbeat225,6600
typedef	struct	_ciscosmds_	_ciscosmds_227,6617
} ciscosmds;ciscosmds230,6686
typedef	struct	_smdshe_	_smdshe_233,6737
}	smdshetype;smdshetype237,6826
#define	SMDSHE_VERLEN	239,6841
#define	SMDSHE_VERTYP	240,6865
#define	SMDSHE_VERVAL	241,6889
#define SMDS_CISCOBYTES 243,6914
#define SMDS_DXIBYTES 244,6973
#define SMDS_SMDSBYTES 245,7030
#define SMDS_BASIZEBYTES 246,7082
#define SMDS_ENCAPBYTES1 247,7128
#define SMDS_ENCAPBYTES2 248,7172
#define SMDSMAXDGRAM 250,7293
#define SMDS_SAP_ENCAPBYTES 252,7369
#define SMDS_SNAP_ENCAPBYTES 253,7429
struct	smds_pdb_t_	smds_pdb_t_279,8631
    MAP 297,9436
#define GLEAN_TIMEOUT_DEF 300,9506
struct smds_gleantype_ smds_gleantype_305,9578