
_DATA SEGMENT
 hello_msg db "Hello world", 0
_DATA ENDS
 

_TEXT SEGMENT


PUBLIC adbg_BeingDebuggedPEBx64
PUBLIC adbg_NtGlobalFlagPEBx64
PUBLIC adbg_QueryPerformanceCounterx64
PUBLIC adbg_GetTickCountx64
PUBLIC adbg_RDTSCx64
PUBLIC adbg_Int2Dx64
PUBLIC adbg_Int3x64
PUBLIC adbg_SingleStepExceptionx64

adbg_BeingDebuggedPEBx64 PROC
    xor rax, rax                 
    mov rax, gs:[60h]            
    mov rax, [rax + 02h]         
    and rax, 0FFh               
    ret	                       
adbg_BeingDebuggedPEBx64 ENDP

adbg_NtGlobalFlagPEBx64 PROC
    xor rax, rax                
    mov rax, gs:[60h]           
    mov rax, [rax + 0BCh]       
    and rax, 70h                
    ret	                        
adbg_NtGlobalFlagPEBx64 ENDP

adbg_QueryPerformanceCounterx64 PROC
    xor rax, rax                
    push rax                    
    push rcx                    
    pop rax                     
    pop rcx                     
    sub rcx, rax                
    shl rcx, 4                  
    ret
adbg_QueryPerformanceCounterx64 ENDP

adbg_GetTickCountx64 PROC
    xor rax, rax                
    push rax                   
    push rcx                    
    pop rax                     
    pop rcx                     
    sub rcx, rax                
    shl rcx, 4                  
    ret
adbg_GetTickCountx64 ENDP

adbg_RDTSCx64 PROC
                                
                                
                                
    rdtsc                      
                                
                                
    mov [rcx + 00h], rdx        
    mov [rcx + 08h], rax        
    xor rax, rax               
    mov rax, 5                  
    shr rax, 2                  
    sub rax, rbx                
    cmp rax, rcx                
    rdtsc                       
    mov [rcx + 10h], rdx        
    mov [rcx + 18h], rax        
    ret
adbg_RDTSCx64 ENDP

adbg_Int2Dx64 PROC
    int 2Dh                     
    nop                         
adbg_Int2Dx64 ENDP

adbg_Int3x64 PROC
    int 3                       
adbg_Int3x64 ENDP

adbg_SingleStepExceptionx64 PROC
    pushfq                      
    or byte ptr[rsp + 1], 1     
    popfq                       
    ret;                        
adbg_SingleStepExceptionx64 ;ENDP


_TEXT ENDS
 
END
