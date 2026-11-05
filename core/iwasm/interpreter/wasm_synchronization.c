#include "wasm_synchronization.h"

//for analyzing only, remove for lockstep implementation later
#include "wasm_exec_env.h" 
#include "wasm_runtime.h"


bool
wasm_synchronization_extract_information(wasm_exec_env_t exec_env, 
    WASMInterpFrame *frame,
    wasm_synchronization_event_t event, 
    wasm_synchronization_info_t *info)
{
    WASMInterpFrame *current;
    uint32_t call_depth = 0;

    info->event = event;

    if (frame != NULL){
        for (current = frame; current != NULL; current = current->prev_frame) {
            call_depth++;
        }

        info->call_depth = call_depth;
        info->operand_depth_cells = (uint32_t)(frame->sp - frame->lp);
        info->control_depth = (uint32_t)(frame->csp - frame->csp_bottom);

        info->ip = frame->ip[-1];
        info->sp = frame->sp;
        info->lp = frame->lp;
        info->stack_depth = (uint32_t)(frame->sp -frame->sp_bottom);

        if (frame->function != NULL) {
            struct WASMModuleInstance *module_inst = (struct WASMModuleInstance *) exec_env->module_inst;

            info->operand_stack_depth_cells = (uint32_t)(frame->sp - frame->lp) - frame->function->param_cell_num - frame->function->local_cell_num;

            info->function_index = (uint32_t)(frame->function - module_inst->e->functions);
            const uint8_t *function_start = wasm_get_func_code(frame->function);
            info->function_start = function_start;
            
            info->bytecode_offset = (int32_t) (info->instruction_start - info->function_start); 

        }
    }
    

    return true;
}