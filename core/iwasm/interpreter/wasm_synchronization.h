#ifndef _WASM_SYNCHRONIZATION_H
#define _WASM_SYNCHRONIZATION_H

#include <stdint.h>
#include <stdbool.h>
#include "wasm_interp.h"

#ifdef _cplusplus
extern "C" {
}
#endif

struct WASMExecEnv;
struct WASMInterpFrame;
struct WASMModuleInstance;
typedef struct WASMExecEnv *wasm_exec_env_t;

typedef struct wasm_synchronization_region{
    uint32_t offset;
    uint32_t size;
} wasm_synchronization_region_t;

typedef enum wasm_synchronization_event{
    WASM_SYNCHRONIZATION_NONE = 0,
    WASM_SYNCHRONIZATION_INSTRUCTION = 1,
    WASM_SYNCHRONIZATION_BEFORE_NATIVE = 2,
    WASM_SYNCHRONIZATION_AFTER_NATIVE = 3,
    WASM_SYNCHRONIZATION_SPECIAL = 4,
    WASM_SYNCHRONIZATION_FUNC_RETURN = 5,
    WASM_SYNCHRONIZATION_FUNC_CALL = 6,
    WASM_SYNCHRONIZATION_STACK_OPERATION = 7,
    WASM_SYNCHRONIZATION_CONTROL_START = 8,
    WASM_SYNCHRONIZATION_CONTROL_END = 9,
    WASM_SYNCHRONIZATION_MEMORY_LOAD = 10,
    WASM_SYNCHRONIZATION_MEMORY_STORE = 11
} wasm_synchronization_event_t;

typedef enum wasm_synchronization_result {
    WASM_SYNCHRONIZATION_CONTINUE = 0,
    WASM_SYNCHRONIZATION_ABORT = 1
} wasm_synchronization_result_t;

#if WAMR_ENABLE_SYNCHRONIZE_STATE_ANALYSIS != 0 && WAMR_ENABLE_SYNCHRONIZE_EVERY_INSTRUCTION == 0
typedef struct wasm_synchronization_info{
    wasm_synchronization_event_t event;
    uint64_t instruction_count; 
    uint8_t ip;
    uint32_t *sp;
    uint32_t function_index;
    uint32_t call_depth;
    uint32_t operand_depth_cells; 
    uint32_t operand_stack_depth_cells;
    uint32_t control_depth;
    int32_t bytecode_offset;
    uint8_t opcode;
    uint8_t *instruction_start;
    uint8_t *function_start;
    uint32_t *lp;
    uint32_t stack_depth;
    uint16_t param_cell;
    uint16_t local_cell;
} wasm_synchronization_info_t;
#endif

#if WAMR_ENABLE_SYNCHRONIZE_EVERY_INSTRUCTION != 0
typedef struct wasm_synchronization_info{
    wasm_synchronization_event_t event;
    uint64_t instruction_count;
    uint8_t opcode;
    uint32_t function_index;
    int32_t bytecode_offset;
    uint32_t *sp;
    uint32_t **frame_sp_exec_env;
    uint32_t *frame_sp_bottom;
    uint32_t stack_depth;
    uint32_t *lp;
    uint32_t locals_depth;
    uint32_t control_depth;
    uint8_t *begin_addr;
    uint8_t *target_addr;
    uint8_t *instruction_start;
    uint8_t *function_start;
    uint8_t *memory_start;
    uint64_t memory_size;
    uint16_t ret_num_native;
    uint32_t argv_ret_native[2];
} wasm_synchronization_info_t;
#endif

typedef wasm_synchronization_result_t (*wasm_synchronization_callback_t)(
    bool * execute_native,
    wasm_synchronization_info_t info, 
    void *worker_data);


bool
wasm_synchronization_extract_information(
    wasm_exec_env_t exec_env,
    WASMInterpFrame *frame,
    wasm_synchronization_event_t event,
    wasm_synchronization_info_t *info
);


#ifdef __cplusplus
}
#endif

#endif /* end of _WASM_SYNCHRONIZATION_H */