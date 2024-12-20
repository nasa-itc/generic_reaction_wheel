/*******************************************************************************
** File: generic_reaction_wheel_checkout.c
**
** Purpose:
**   This checkout can be run without cFS and is used to quickly develop and 
**   test functions required for a specific component.
**
*******************************************************************************/

/*
** Include Files
*/
#include "generic_reaction_wheel_checkout.h"

/*
** Global Variables
*/
static uart_info_t RW_UART[3] = {
    {.deviceString = &GENERIC_REACTION_WHEEL_1_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_1_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_1_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_1_CFG_BAUDRATE_HZ},
    {.deviceString = &GENERIC_REACTION_WHEEL_2_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_2_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_2_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_2_CFG_BAUDRATE_HZ},
    {.deviceString = &GENERIC_REACTION_WHEEL_3_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_3_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_3_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_3_CFG_BAUDRATE_HZ},
};
double RwData;

/*
** Component Functions
*/
void print_help(void) 
{
    printf(PROMPT "command [args]\n"
        "---------------------------------------------------------------------\n"
        "help                               - Display help                    \n"
        "exit                               - Exit app                        \n"
        "get_momentum                      - Request rw momentum            \n"
        "  m                                - ^                               \n"
        "set_torque x y z                   - Send torque for RWs             \n"
        "  t x y z                          - ^                               \n"
        "\n"
    );
}


int get_command(const char* str)
{
    int status = CMD_UNKNOWN;
    char lcmd[MAX_INPUT_TOKEN_SIZE];
    strncpy(lcmd, str, MAX_INPUT_TOKEN_SIZE);

    /* Convert command to lower case */
    to_lower(lcmd);

    if(strcmp(lcmd, "help") == 0) 
    {
        status = CMD_HELP;
    }
    else if(strcmp(lcmd, "exit") == 0) 
    {
        status = CMD_EXIT;
    }
    else if(strcmp(lcmd, "get_momentum") == 0) 
    {
        status = CMD_GET_MOMENTUM;
    }
    else if(strcmp(lcmd, "m") == 0) 
    {
        status = CMD_GET_MOMENTUM;
    }
    else if(strcmp(lcmd, "set_torque") == 0) 
    {
        status = CMD_SET_TORQUE;
    }
    else if(strcmp(lcmd, "t") == 0) 
    {
        status = CMD_SET_TORQUE;
    }
    return status;
}


int process_command(int cc, int num_tokens, char tokens[MAX_INPUT_TOKENS][MAX_INPUT_TOKEN_SIZE])
{
    int32_t status = OS_SUCCESS;
    int32_t exit_status = OS_SUCCESS;
    double torque;

    /* Process command */
    switch(cc) 
    {	
        case CMD_HELP:
            print_help();
            break;
        
        case CMD_EXIT:
            exit_status = OS_ERROR;
            break;

        case CMD_GET_MOMENTUM:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                for ( int i = 0; i < 3; i++)
                {
                    status = GetCurrentMomentum(&RW_UART[i], &RwData);
                    if (status < 0)
                    {
                        OS_printf("GENERIC_REACTION_WHEEL_RequestData command failed for RW %d!\n", i);
                    }
                    else
                    {
                        OS_printf("RW_GetCurrentMomentum: Success for RW %d! Momentum: %lf\n", i, RwData);
                    }
                }
            }
            break;

        case CMD_SET_TORQUE:
            if (check_number_arguments(num_tokens, 3) == OS_SUCCESS)
            {
                for (int i = 0; i < 3; i++)
                {
                    torque = atof(tokens[i]);
                    status = SetRWTorque(&RW_UART[i], torque);
                    if (status < 0)
                    {   
                        OS_printf("GENERIC_REACTION_WHEEL_SetTorque command failed for RW %d!\n", i);
                    }
                    else
                    {
                        OS_printf("RW %d torque successfully set to %lf\n", i, torque);
                    }
                }
            }
            break;
        
        default: 
            OS_printf("Invalid command format, type 'help' for more info\n");
            break;
    }
    return exit_status;
}


int main(int argc, char *argv[]) 
{
    int status = OS_SUCCESS;
    char input_buf[MAX_INPUT_BUF];
    char input_tokens[MAX_INPUT_TOKENS][MAX_INPUT_TOKEN_SIZE];
    int num_input_tokens;
    int cmd;    
    char* token_ptr;
    uint8_t run_status = OS_SUCCESS;

    /* Initialize HWLIB */
    #ifdef _NOS_ENGINE_LINK_
        nos_init_link();
    #endif

     /* Connect to the UART */
    status = uart_init_port(&RW_UART[0]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 0 port initialization error!\n");
    }    
    status = uart_init_port(&RW_UART[1]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 1 port initialization error!\n");
    }    
    status = uart_init_port(&RW_UART[2]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 2 port initialization error!\n");
    }    

    /* Main loop */
    print_help();
    while(run_status == OS_SUCCESS) 
    {
        num_input_tokens = -1;
        cmd = CMD_UNKNOWN;

        /* Read user input */
        printf(PROMPT);
        fgets(input_buf, MAX_INPUT_BUF, stdin);

        /* Tokenize line buffer */
        token_ptr = strtok(input_buf, " \t\n");
        while((num_input_tokens < MAX_INPUT_TOKENS) && (token_ptr != NULL)) 
        {
            if(num_input_tokens == -1) 
            {
                /* First token is command */
                cmd = get_command(token_ptr);
            }
            else 
            {
                strncpy(input_tokens[num_input_tokens], token_ptr, MAX_INPUT_TOKEN_SIZE);
            }
            token_ptr = strtok(NULL, " \t\n");
            num_input_tokens++;
        }

        /* Process command if valid */
        if(num_input_tokens >= 0)
        {
            /* Process command */
            run_status = process_command(cmd, num_input_tokens, input_tokens);
        }
    }

    // Close the devices
    for ( int i = 0; i < 3; i++ )
    {
        uart_close_port(&RW_UART[i]);
    }

    #ifdef _NOS_ENGINE_LINK_
        nos_destroy_link();
    #endif

    OS_printf("Cleanly exiting generic_reaction_wheel application...\n\n"); 
    return 1;
}


/*
** Generic Functions
*/
int check_number_arguments(int actual, int expected)
{
    int status = OS_SUCCESS;
    if (actual != expected)
    {
        status = OS_ERROR;
        OS_printf("Invalid command format, type 'help' for more info\n");
    }
    return status;
}

void to_lower(char* str)
{
    char* ptr = str;
    while(*ptr)
    {
        *ptr = tolower((unsigned char) *ptr);
        ptr++;
    }
    return;
}

