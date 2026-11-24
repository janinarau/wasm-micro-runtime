#include "wasm_synchronization.h"
#include "wasm_runtime.h"

#if WAMR_ENABLE_SYNCHRONIZE_STATE_ANALYSIS != 0 && WAMR_ENABLE_SYNCHRONIZE_EVERY_INSTRUCTION == 0
#include "wasm_exec_env.h" 


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
#endif

#if WAMR_ENABLE_SYNCHRONIZE_EVERY_INSTRUCTION != 0
bool
wasm_synchronization_extract_information(wasm_exec_env_t exec_env, 
    WASMInterpFrame *frame,
    wasm_synchronization_event_t event, 
    wasm_synchronization_info_t *info)
{
    info->event = event;

    if (frame != NULL){
        
        info->event = event;

        info->locals_depth = (uint32_t)(frame->sp - frame->lp);
        info->control_depth = (uint32_t)(frame->csp - frame->csp_bottom);
        info->stack_depth = (uint32_t)(frame->sp -frame->sp_bottom);
        info->frame_sp_bottom = frame->sp_bottom;

        info->sp = frame->sp;
        info->lp = frame->lp;

        if (event == WASM_SYNCHRONIZATION_AFTER_NATIVE){
            info->ret_num_native = exec_env->ret_num_native;
            info->argv_ret_native[0] = exec_env->argv_ret_native[0];
            info->argv_ret_native[1] = exec_env->argv_ret_native[1];
            info->frame_sp_exec_env = &frame->sp; 
        }

        /* frame->csp points to next free entry, control stack is never empty */
        info->begin_addr = (frame->csp-1)->begin_addr;
        info->target_addr = (frame->csp-1)->target_addr;

        if (frame->function != NULL) {

            info->function_start = (uint8_t*) wasm_get_func_code(frame->function);
            
            info->bytecode_offset = (int32_t) (info->instruction_start - info->function_start); 

        }
    } else {
        return false;
    }
    

    return true;
}
#endif