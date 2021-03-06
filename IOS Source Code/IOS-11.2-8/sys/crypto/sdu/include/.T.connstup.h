
connstup.h,4427
#define CONNSTUP_H210,7260
typedef ulong   time_t;time_t217,7471
#define SECURE_LAN_PRTCL 222,7640
#define PS_SDU 224,7671
#define PS_SFU 225,7733
#define PS_SNEIC 226,7795
#define ECHO_REQUEST 228,7866
#define ECHO_REPLY 229,7900
#define ECHO_CONFIRM 230,7934
#define CS_NO_ERROR 232,7969
#define CS_ERR_NO_TX_BUF 233,8039
#define CS_ERR_NO_PCONN_ENTRY 234,8126
#define CS_ERR_PCONN_TIMEOUT 235,8218
#define CS_ERR_NNC_EXP 236,8321
#define CS_ERR_NNC_SIG 237,8416
#define CS_ERR_SU_SIG 238,8507
#define CS_ERR_PCONN_INDEX_LOST 239,8605
#define CS_ERR_CHALLENGE_ODD 240,8676
#define CS_ERR_CHALLENGE_CMP 241,8800
#define CS_ERR_NNC_GRPS 242,8923
#define CS_ERR_GEN_CHL 243,9028
#define CS_ERR_GEN_DH 244,9135
#define CS_ERR_GEN_SIG 245,9245
#define CS_ERR_ALLOC_HM 246,9345
#define CS_ERR_RELEASE_HM 247,9449
#define CS_ERR_CY_KEYS 248,9556
#define CS_ERR_SOK_MASK_RNG 249,9656
#define CS_ERR_VERIFY_SIG 250,9776
#define CS_ERR_NNC_FRM_SIZE 251,9886
#define CS_ERR_TRANSMIT 252,9995
#define CS_ERR_AUTH_API	253,10085
#define CS_ERR_NC_NNC_OUT_SYNC	254,10166
#define CS_ERR_CASCADED_CONFIG	255,10309
#define CS_ERR_NNC_SIZE	256,10426
#define CS_ERR_NVM_PUTREC	257,10520
#define	CS_EVT_CONFIG_TB_MISSING	258,10611
#define INIT_CHLG_FAILED 261,10716
#define STAT_AUTH_MASK 270,10984
#define AUTH_NNC_SIG_FAILED 271,11061
#define AUTH_SDU_SIG_FAILED 272,11138
#define AUTH_CHALLENGE_FAILED 273,11203
#define STAT_CYOC_MASK 275,11279
#define STAT_GEN_DH_FAILED 276,11359
#define STAT_GEN_CHL_FAILED 277,11438
#define STAT_GEN_SIG_FAILED 278,11518
#define STAT_SYS_OC_FLAG 279,11600
#define INIT_ICMP_ECHO_REQ 281,11666
#define INIT_ICMP_ECHO_RPY 282,11700
#define PCONN_ENTRIES 284,11735
#define MAX_PCONN_TIME 287,11797
#define MAX_PCONN_TIME 289,11882
#define ICMP_HEADER_SIZE 292,11957
#define ECHO_HEADER_SIZE 293,11991
#define AEP_HEADER_SIZE 294,12025
#define CHALLENGE_SIZE 296,12060
#define CHAL_ODD_MASK 297,12095
#define CHAL_EVEN_MASK 298,12132
#define NULL_INDEX 300,12170
#define UNDEF_INDEX 301,12205
#define MAC_ADDR_LEN 302,12240
#define MAC_CRC32_SIZE	303,12274
#define MAC_TO_DSAP_LEN 304,12301
#define MAC_TO_SSAP_LEN 305,12336
#define IP_ADDR_LEN 306,12371
#define RAND_GEN_SIZE	307,12405
#define MAX_DSS_SIZE 310,12473
#define DSS_FIX_SIZE 311,12508
#define DEF_SIG_SIZE 314,12623
#define CY_KEY_SIZE 315,12658
#define SKEY_OFFSET 316,12692
#define IKEY_OFFSET 317,12726
#define NNC_GRP_BLK_SIZE	318,12761
#define SERIAL_NUM_SIZE 322,12902
#define PDU_NW_ADDR_SIZE 323,12937
#define	DDP_NW_ADDR_SIZE	324,12972
#define PCONN_INDEX_SIZE 325,13001
#define PDU_MAX_NW_ADDR_SIZE	326,13036
#define NNC_GRP_VEC_OFFSET 328,13070
#define NNC_GRP_OC_OFFSET 329,13104
#define NNC_CY_VEC_OFFSET 330,13138
#define NNC_CUS_ID_OFFSET 331,13173
#define NNC_FIX_GRP_OFFSET 332,13207
#define NNC_ENTITY_ID_OFFSET	333,13270
#define NNC_REV_OFFSET 334,13301
#define NNC_FIX_SIZE	335,13336
#define PDU_FIX_NNC_OFFSET 336,13404
#define PDU_CHAL_OFFSET 337,13466
#define PDU_FIX_NNC_REV_OFFSET 338,13501
#define PDU_PS_OFFSET 339,13563
#define PDU_OP_OFFSET 340,13597
#define PDU_STAT_OFFSET 341,13636
#define PDU_SERIAL_NUM_OFFSET 342,13670
#define PDU_NW_ADDR_OFFSET	343,13704
#define PDU_INIT_CONNID_OFFSET 344,13734
#define PDU_RESP_CONNID_OFFSET	345,13769
#define PDU_INIT_PCONN_OFFSET 346,13803
#define PDU_RESP_PCONN_OFFSET 347,13838
#define PDU_CFM_RP_CID_OFFSET	348,13873
#define PDU_CFM_RPC_OFFSET 349,13905
#define PDU_CFM_CHAL_OFFSET 350,13940
#define PDU_CFM_SIG_OFFSET	351,13975
#define MAX_RETX_HEADER 353,14007
#define DEFAULT_RETRANSMISSIONS 355,14115
#define MAX_PRTCLS 358,14220
#define MAX_GRP_BLKS 359,14255
#define NNC_GV_BLK_SIZE 360,14289
#define MAX_NNC_SIZE 361,14324
#define MAX_GRPS 362,14397
#define SPECIAL_NNCS	364,14529
#define MAX_NNC_BUF_SIZE 365,14582
#define SNMP_ROW_ACTIVE	367,14660
#define SNMP_ROW_CREATE_GO	368,14689
#define SNMP_ROW_DESTROYED	369,14720
#define SYNDROME_HMA_OFFSET	371,14752
pconn_entry;pconn_entry409,16348
entity_pair;entity_pair417,16465
srng_tb_def;srng_tb_def427,16634
#define NOTUSED	444,17064
#define ACTIVE	445,17084
#define NOTINSERVICE	446,17104
#define NOTREADY	447,17128
#define CREATEANDGO	448,17149
#define CREATEANDWAIT	449,17172
#define DESTROY	450,17196
#define REJECT	453,17285
#define PASS	454,17305
#define ENCRYPT	455,17323
