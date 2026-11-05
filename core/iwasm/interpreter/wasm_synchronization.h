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
    WASM_SYNCHRONIZATION_SPECIAL = 4
} wasm_synchronization_event_t;

typedef enum wasm_synchronization_result {
    WASM_SYNCHRONIZATION_CONTINUE = 0,
    WASM_SYNCHRONIZATION_ABORT = 1
} wasm_synchronization_result_t;

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

typedef wasm_synchronization_result_t (*wasm_synchronization_callback_t)(
    wasm_exec_env_t exec_env, 
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