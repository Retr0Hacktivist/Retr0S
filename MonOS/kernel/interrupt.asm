[BITS 32]
[GLOBAL keyboard_handler]
[EXTERN keyboard_handler_c]

keyboard_handler:
    pusha              ; sauvegarder tous les registres
    call keyboard_handler_c
    popa               ; restaurer les registres
    iret               ; retour d'interruption
