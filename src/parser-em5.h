#ifndef EM5_PARSER_H
#define EM5_PARSER_H

/* Workaround for gcc -Wunused-variable */
#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#include "em.h"
#include <stdbool.h>

enum parser_em5_ret{
	RET_OK
	, RET_EVENT
	, RET_SYNC
	, RET_END_SPILL
	, RET_WARNING
	, WARN_DMA_OVERREAD
	, WARN_MISS_ADDR_ORDER
	, RET_ERROR
	, ERR_DUP
	, ERR_ZEROES
	, ERR_ONES
	, ERR_UNKNOWN_WORD
	, ERR_PROTOCOL
	, ERR_MISS_LEN
	, ERR_MISS_DUP_ADDR
	, MAX_EM5_PARSER_RET  // the last element
	};


static const char UNUSED *parser_em5_retstr[] = {
	[RET_OK] = "-"
	, [RET_EVENT] = "CNT_EM_EVENT"
	, [RET_SYNC] = "CNT_EM_SYNC_EVENT"
	, [RET_END_SPILL] = "CNT_END_SPILL"
	, [WARN_DMA_OVERREAD] = "WARN_KNOWN_DMA_OVERREAD"  // DMA read full burst when no more data in buffer (always after 0xFE)
	, [WARN_MISS_ADDR_ORDER] = "WARN_MISS_ADDR_ORDER"	// MISS addresses not ascending during sequential readout.
	, [ERR_DUP] = "ERR_EM_DUPWORD"	// Duplicate word.
	, [ERR_ZEROES] = "ERR_EM_ZERO_WORD"	// A zero word.
	, [ERR_ONES] = "ERR_EM_ONES_WORD"	// A word with all ones.
	, [ERR_UNKNOWN_WORD] = "ERR_EM_UNKNOWN_WORD"	// Unknown word type.
	, [ERR_PROTOCOL] = "ERR_PROTOCOL"  // EM5 protocol error
//	, [ERR_NO_FE]= "ERR_EM_NO_FE"	// Sudden new event (0xBE).
//	, [ERR_NO_BE]= "ERR_EM_NO_BE"	// Sudden event tail (0xFE).
//	, [ERR_NO_1F] = "ERR_EM_NO_1F"	// Missing stats word (0x1F).
	, [ERR_MISS_LEN] = "ERR_MISS_LEN"	// MISS event len counter != actual lengh.
	, [ERR_MISS_DUP_ADDR] = "ERR_MISS_DUP_ADDR"  // same MISS module twice in one event
	};


enum em5_protocol_state {
	NO_STATE
	, PCHI_BEGIN  // sequential data readout 
	, PCHN_BEGIN  // sequential module position numbers readout
	, PCH_DATA
	, PCH_END  // end of pchi or pchn
	};

static const char UNUSED * em5_protocol_state_str[] = {
	[NO_STATE] = "-"
	, [PCHI_BEGIN] = "PCHI"
	, [PCHN_BEGIN] = "PCHN"
	, [PCH_DATA] = "data"
	, [PCH_END] = "END"
	};

enum emword_class {
	WORD_UNKNOWN
	, WORD_BEGIN_SPILL
	, WORD_END_SPILL
	, WORD_BEGIN_EVENT
	, WORD_BEGIN_ENUM
	, WORD_STAT_1F
	, WORD_END_EVENT
	, WORD_DATA
	, WORD_SYNC
	, WORD_ZERO
	, WORD_ONES
	, WORD_DUP	
	};

static const char UNUSED * emword_class_str[] = {
	[WORD_UNKNOWN] = "UNKNOWN"
	, [WORD_BEGIN_SPILL] = "BS"
	, [WORD_END_SPILL] = "ES"
	, [WORD_BEGIN_EVENT] = "EVENT"
	, [WORD_BEGIN_ENUM] = "ENUM"
	, [WORD_STAT_1F] = "STAT"
	, [WORD_END_EVENT] = "END"
	, [WORD_DATA] = "DATA"
	, [WORD_SYNC] = "SYNC"
	, [WORD_ZERO] = "ZERO"
	, [WORD_ONES] = "ONES"
	, [WORD_DUP] = "DUPLICATE"
};

struct parser_em5 {
	emword prev;  // previous word
	unsigned last_sync_ts;  // last sync event timestamp
	enum em5_protocol_state state;  // current readout protocol
	unsigned prev_evt_ts;  // timestamp of a previous event
	struct parser_em5_event_info {
		unsigned ts;  // timestamp
		unsigned len; // length in words
		unsigned len_1f; // length according to MISS
		unsigned prev_mod;  // previous module position number
		unsigned cnt;  // event word counter
		bool dirty; // event is dirty
		bool err_ovf; // overflow error during readout
		bool err_to;  // timeout error during readout
		bool err_miss;  // general miss error
		unsigned woff;  //offset in words
		unsigned mod_offt[EM_MAX_MODULE_NUM];  // event data offset FIXME:del
		unsigned mod_cnt[EM_MAX_MODULE_NUM];  // word counter per module  FIXME:del
		} evt; 
	unsigned ret_cnt[MAX_EM5_PARSER_RET];  // return value counters
	unsigned word_cnt;  // word counter since init
	unsigned evt_cnt;  // event_counter
	unsigned dirty_cnt;  // dirty events couner
	};

enum parser_em5_ret parser_em5_next(struct parser_em5 *, emword);

#endif /* EM5_PARSER_H */