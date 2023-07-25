#include "digital.h"
#include "bsp.h"
#include "chip.h"
#include "poncho.h"
#include "ciaa.h"
//Las funcion que seran usadas como funciones de callback, van en funciones publcias o privadas

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */
static struct board_s board = {0};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

//! Funcion para iniciar placa
void InitBoard(void);

//! Funcion para configurar el hardware de los digitos
void DigitsInit (void);


//! Funcion para configurar el hardware de los segmentos
void SegmentsInit(void);


//! Funcion para configurar el hardware del buzzer
void BuzzerInit(void);


//! Funcion para congifu
void KeysInit(void);


//! Funcion para apagar todos los segmentos y digitos
void ScreenTurnOff(void);


/**
 * @brief Funcion para prender solo los segmentos seleccinados
 * 
 * @param segments Marcara para seleccionar los segmentos
 */
void SegmnetsTurnOn(uint8_t segments);


/**
 * @brief Funcion para prender solo el digito seleccionado
 * 
 * @param digit numero de digito a prender
 */
void DigitTurnOn(uint8_t digit);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */
void InitBoard(void){
    // Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, true);

    // Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);

    // Chip_SCU_PinMuxSet(LED_1_PORT, LED_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_1_FUNC);
    // DigitalOutputCreate(LED_1_GPIO, LED_1_BIT);

    // Chip_SCU_PinMuxSet(LED_2_PORT, LED_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_2_FUNC);
    // DigitalOutputCreate(LED_2_GPIO, LED_2_BIT);

    // Chip_SCU_PinMuxSet(LED_3_PORT, LED_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_3_FUNC);
    // DigitalOutputCreate(LED_3_GPIO, LED_3_BIT);
}




void DigitsInit(void){

    Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, true);

}

void SegmentsInit(void){

    Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, true);

}

void BuzzerInit(void){

    // Chip_SCU_PinMuxSet(BUZZER_PORT, BUZZER_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | BUZZER_FUNC);
    // board.buzzer = DigitalOutputCreate(BUZZER_GPIO, BUZZER_BIT);

    Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    board.buzzer = DigitalOutputCreate(LED_R_GPIO, LED_R_BIT);


}

void KeysInit (void){

    Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F1_FUNC);
    board.set_time = DigitalInputCreate(KEY_F1_GPIO,KEY_F1_BIT);

    Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F2_FUNC);
    board.set_alarma = DigitalInputCreate(KEY_F2_GPIO,KEY_F2_BIT);

    Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F3_FUNC);
    board.increment = DigitalInputCreate(KEY_F3_GPIO,KEY_F3_BIT);

    Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F4_FUNC);
    board.decrement = DigitalInputCreate(KEY_F4_GPIO,KEY_F4_BIT);

    Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_CANCEL_FUNC);
    board.cancel = DigitalInputCreate(KEY_CANCEL_GPIO,KEY_CANCEL_BIT);

    Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_ACCEPT_FUNC);
    board.accept = DigitalInputCreate(KEY_ACCEPT_GPIO,KEY_ACCEPT_BIT);

}

void ScreenTurnOff(void){

    Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
}

void SegmentsTurnOn(uint8_t segments){

    Chip_GPIO_SetValue(LPC_GPIO_PORT,SEGMENTS_GPIO, (segments) & SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (segments & SEGMENT_P));     //ver!!!!!!!!!1
}

void DigitTurnOn(uint8_t digit){

    Chip_GPIO_SetValue(LPC_GPIO_PORT,DIGITS_GPIO, (1<<(digit))& DIGITS_MASK); // Ver si no hace falta poner 1<<(3-digit)

}

/* === Public function implementation ========================================================= */


board_t BoardCreate(void){
    static const struct display_driver_s driver[1] = {
        {
        .ScreenTurnOff = ScreenTurnOff,
        .SegmentsTurnOn = SegmentsTurnOn,
        .DigitTurnOn = DigitTurnOn,
        }
    };

    InitBoard();
    DigitsInit();
    SegmentsInit();
    BuzzerInit();
    KeysInit();

    board.display = DisplayCreate(4,driver);

	return &(board);
}




void SisTick_Init(uint16_t ticks){
	__asm volatile("cpsid i");

	// Activo el sistick
	SystemCoreClockUpdate(); //Deshabilito interrupciones
	SysTick_Config(SystemCoreClock/ticks); //Actualiza una variable global para configurar la cantidad de interrupciones por segundo

	//NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1); //Fijo la prioridad de la interrupcion
	
	__asm volatile("cpsie i");
}