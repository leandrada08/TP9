

#include "bsp.h"
#include "reloj.h"
#include "bcd.h"
#include <stdbool.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Definicion de macros*/
#ifndef TICS_POR_SEGUNDO
#define TICS_POR_SEGUNDO 1000
#endif

#ifndef PERIODO_PARPADEO
#define PERIODO_PARPADEO 200
#endif

#ifndef POSPONER_MINUTOS
#define POSPONER_MINUTOS 5
#endif

#ifndef MODIFICAR
#define MODIFICAR 3*TICS_POR_SEGUNDO
#endif

#ifndef CANCELAR_MODIFICAR
#define CANCELAR_MODIFICAR MODIFICAR*10
#endif


// Tamaño de pila para las tareas que lo requieran
#define TAMANO_PILA_TAREA_PRINCIPAL 512
#define TAMANO_PILA_TAREA_TECLADO 256
#define TAMANO_PILA_TAREA_SYSTICK 128
#define TAMANO_PILA_TAREA_REFRESCO 128

// Defino prioridades de las tareas
#define PRIORIDAD_TAREA_PRINCIPAL (tskIDLE_PRIORITY + 3)
#define PRIORIDAD_TAREA_TECLADO (tskIDLE_PRIORITY + 2)
#define PRIORIDAD_TAREA_SYSTICK (tskIDLE_PRIORITY + 1)
#define PRIORIDAD_TAREA_REFRESCO (tskIDLE_PRIORITY)

// Crear variables para las tareas
TaskHandle_t xTareaPrincipalHandle;
TaskHandle_t xTareaTecladoHandle;
TaskHandle_t xTareaSysTickHandle;
TaskHandle_t xTareaRefrescoHandle;

// Defino tamanio de la cola
#define TAMANO_COLA_TECLADO 10



/* Delacacion de datos privados*/
typedef enum{
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTANDO_MINUTOS_ACTUAL,
    AJUSTANDO_HORAS_ACTUAL,
    AJUSTANDO_MINUTOS_ALARMA,
    AJUSTANDO_HORAS_ALARMA,
} modo_t;


typedef enum {
    TECLA_ACEPTAR,
    TECLA_CANCELAR,
    TECLA_INCREMENTAR,
    TECLA_DECREMENTAR,
    TECLA_TIME,
    TECLA_ALARMA,
    EVENTO_3STIME,
    EVENTO_3SALARMA,
    EVENTO_30S
} evento_t;

/*Declaracion de funciones Privadas*/

/**
 * @brief Funcion para activar el buzzer de la alarma
 * 
 * @param reloj Reloj al cual le activaremos la alarma
 */
void ToggleAlarma(clk_t reloj);

/**
 * @brief Funcion para cambiar el estado en el que nos encontramos y actualizar la pantalla al correspondiente estado
 * 
 * @param valor modo en el que nos encontramos
 */
void CambiarModo(modo_t valor);

static void TareaPrincipal(void* pvParameters);
static void TareaTeclado(void* pvParameters);
static void DisplayRefreshTask(void * pvParameters);
static void TareaSysTick(void* Parameters);


/*Definicion de variable globales*/
static board_t board;
static clk_t reloj;
static modo_t modo;
static bool sonando_alarma = false;
static const uint8_t LIMITE_MINUTO[]={6,0};
static const uint8_t LIMITE_HORA[]={2,4};
static uint16_t tiempo_presionado_hora_actual;
static uint16_t tiempo_presionado_hora_alarma;
static uint16_t tiempo_sin_presionar; 

/* Definicion de colas */
xQueueHandle colaTeclado;


/*Imprementacion de funciones publicas*/
void ToggleAlarma(clk_t reloj){
    if(sonando_alarma){
        sonando_alarma=false;
        DigitalOutputDesactivate(board->buzzer);
    }else{
        sonando_alarma=true;
        DigitalOutputActivate(board->buzzer);
    }
}

void CambiarModo(modo_t valor){
    modo = valor;
    switch (modo)
    {
    case SIN_CONFIGURAR:
        DisplayFlashDigits(board->display,0,3,PERIODO_PARPADEO);
        break;
    case MOSTRANDO_HORA:
        DisplayFlashDigits(board->display, 0, 0, 0);
        break;
    case AJUSTANDO_MINUTOS_ACTUAL:
        DisplayFlashDigits(board->display, 0, 1, PERIODO_PARPADEO);
        break;
    case AJUSTANDO_HORAS_ACTUAL:
        DisplayFlashDigits(board->display, 2, 3, PERIODO_PARPADEO);
        break;
    case AJUSTANDO_MINUTOS_ALARMA:
        DisplayFlashDigits(board->display, 0, 1, PERIODO_PARPADEO);
        break;
    case AJUSTANDO_HORAS_ALARMA:
        DisplayFlashDigits(board->display, 2, 3, PERIODO_PARPADEO);
        break;  
    default:
        break;
    }
}




void TareaPrincipal(void* Parameters) {
    uint8_t entrada[4]; // Vector en el cual guardaremos el valor de entrada y lo que mostraremos mientras configuro la hora
    evento_t dato;

    while (1) {
        // Leer el valor del teclado de la cola
        if (xQueueReceive(colaTeclado, &dato, portMAX_DELAY) == pdTRUE) {

            switch (dato) {

                case TECLA_ACEPTAR:
                    if(sonando_alarma){
                        PosponerAlarma(reloj,POSPONER_MINUTOS,sonando_alarma);
                        //ToggleAlarma(reloj);
                    }else{
                        if(modo==MOSTRANDO_HORA){
                            ClkActivateAlarma(reloj,1);
                        }else
                        if(modo==AJUSTANDO_MINUTOS_ACTUAL){//<! Si estoy configurando los minutos y apreto acceptar paso a configurar la hora
                            CambiarModo(AJUSTANDO_HORAS_ACTUAL);
                        }else
                        if(modo==AJUSTANDO_HORAS_ACTUAL){
                            ClkSetTime(reloj, entrada, sizeof(entrada));//<! Si estoy configurando la hora y apreto aceptar, termino de configurar quedo mostrando hora
                            CambiarModo(MOSTRANDO_HORA);
                        }else
                        if(modo==AJUSTANDO_MINUTOS_ALARMA){//<! Si estoy configurando los minutos y apreto acceptar paso a configurar la hora
                            CambiarModo(AJUSTANDO_HORAS_ALARMA);
                        }else
                        if(modo==AJUSTANDO_HORAS_ALARMA){
                            ClkSetAlarma(reloj, entrada, sizeof(entrada));//<! Si estoy configurando la hora y apreto aceptar, termino de configurar quedo mostrando hora
                            CambiarModo(MOSTRANDO_HORA);
                        }
                    }
                    break;



                case TECLA_CANCELAR:
                    if(sonando_alarma){
                        ToggleAlarma(reloj);
                    }else{
                        if(modo==MOSTRANDO_HORA){
                            ClkActivateAlarma(reloj,0);
                        }else{
                            if(ClkGetTime(reloj, entrada, sizeof(entrada))){//Si me devuelve 1 es porque es valida y 0 porque es invalida
                                CambiarModo(MOSTRANDO_HORA);//Si la hora es valida y cancelo la configuracion de alarma u hora, vuelvo a mostrar hora
                            }else{
                                CambiarModo(SIN_CONFIGURAR); //Si la hora es invalida y cancelo la configuracion, vuelvo a sin configurar
                            }
                        }
                    }
                    break;



                case TECLA_INCREMENTAR:
                    if((modo==AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){
                        IncrementarBCD(entrada,LIMITE_HORA);
                        DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    }else
                    if((modo==AJUSTANDO_MINUTOS_ACTUAL)||(modo==AJUSTANDO_MINUTOS_ALARMA)){
                        IncrementarBCD(&entrada[2],LIMITE_MINUTO);
                        DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    }
                    break;


                case TECLA_DECREMENTAR:
                    if((modo==AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){
                        DecrementarBCD(entrada,LIMITE_HORA);
                        DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    }else
                    if((modo==AJUSTANDO_MINUTOS_ACTUAL)||(modo==AJUSTANDO_MINUTOS_ALARMA)){
                        DecrementarBCD(&entrada[2],LIMITE_MINUTO);
                        DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    }
                    break;


                case TECLA_TIME:
                        tiempo_presionado_hora_actual=1;
                    break;


                case TECLA_ALARMA:
                        tiempo_presionado_hora_alarma=1;
                    break;


                case EVENTO_3STIME:
                    tiempo_presionado_hora_actual=0;
                    tiempo_sin_presionar=1;
                    CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
                    ClkGetTime(reloj, entrada, sizeof(entrada));
                    DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    break;


                case EVENTO_3SALARMA:
                    tiempo_presionado_hora_alarma=0;
                    tiempo_sin_presionar=1;
                    CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
                    ClkGetAlarma(reloj, entrada, sizeof(entrada));
                    DisplayWriteBCD(board->display,entrada,sizeof(entrada));
                    break;



                case EVENTO_30S:
                    tiempo_sin_presionar = 0;
                    CambiarModo(MOSTRANDO_HORA);
                    break;


                default:
                    break;
            }
        }
    }
}

            


void TareaTeclado(void* Parameters) {
    evento_t entrada;

    while (1) {
        if(DigitalInputGetState(board->accept)){
            entrada = TECLA_ACEPTAR; 
        }
        if(DigitalInputGetState(board->cancel)){
            entrada = TECLA_CANCELAR;
        }
        if(DigitalInputGetState(board->increment)){
            entrada = TECLA_INCREMENTAR;
        }

        if(DigitalInputGetState(board->decrement)){
            entrada = TECLA_DECREMENTAR;
        }

        if(DigitalInputGetState(board->set_time)){
            entrada = TECLA_TIME;
        }

        if(tiempo_presionado_hora_actual>MODIFICAR){
            entrada = EVENTO_3STIME;
        }
        if(DigitalInputGetState(board->set_alarma)){
            entrada = TECLA_ALARMA;
        }

        if(tiempo_presionado_hora_alarma>MODIFICAR){
            entrada = EVENTO_3SALARMA;
        }

        if (tiempo_sin_presionar > CANCELAR_MODIFICAR){
            entrada = EVENTO_30S;
        }

        // Enviar la entrada a la cola
        xQueueSend(colaTeclado, &entrada, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(150));
    }
}











static void TareaSysTick(void* Parameters){
    TickType_t last_value = xTaskGetTickCount();
    static uint16_t contador=0;
    uint8_t hora[4];
    bool llave=false;
    DisplayRefresh(board->display);
    ClkTick(reloj);

    // Contador para que el punto central parpadee 1 vez por segundo
    contador = (contador+1)%TICS_POR_SEGUNDO;
    if(contador>=TICS_POR_SEGUNDO/2){
        llave=true;
    }else{
        llave=false;
    }


    // Logica para parpadear punto central y prender ultimo punto
    if(modo<=MOSTRANDO_HORA){ //Solo muestra hora en mostrando hora y configurar
        ClkGetTime(reloj, hora, sizeof(hora));
        DisplayWriteBCD(board->display,hora, sizeof(hora));
        if(llave){
            DisplayToggleDot(board->display,1);
        }
        if(ClkGetAlarma(reloj, hora, sizeof(hora))){
            DisplayOnDot(board->display,3);
        }
    }


    // Logica para prender todos los puntos cuando se este modificando el codigo
    if(modo==AJUSTANDO_MINUTOS_ALARMA||modo==AJUSTANDO_HORAS_ALARMA){
       DisplayOnDot(board->display,0);
       DisplayOnDot(board->display,1);
       DisplayOnDot(board->display,2);
       DisplayOnDot(board->display,3); 
    }



    // Logica para contar tiempo de boton presionado
    if(tiempo_presionado_hora_actual>0 && DigitalInputGetState(board->set_time)){
        tiempo_presionado_hora_actual++;
    } else{
        tiempo_presionado_hora_actual= 0;
    }
    if(tiempo_presionado_hora_alarma>0 && DigitalInputGetState(board->set_alarma)){
        tiempo_presionado_hora_alarma++;
    } else{
        tiempo_presionado_hora_alarma=0;
    }
    if( (tiempo_sin_presionar>0) &&
        !DigitalInputGetState(board->accept) &&
        !DigitalInputGetState(board->cancel) &&
        !DigitalInputGetState(board->decrement) &&
        !DigitalInputGetState(board->increment)){
        tiempo_sin_presionar++;
    }

    vTaskDelayUntil(&last_value, pdMS_TO_TICKS(1));
}


static void DisplayRefreshTask(void * pvParameters) {
    while (true) {
        DisplayRefresh(board->display);
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}


int main(void) {

    reloj =ClkCreate(TICS_POR_SEGUNDO, ToggleAlarma);
    board = BoardCreate();
    CambiarModo(SIN_CONFIGURAR);

    SisTick_Init(TICS_POR_SEGUNDO);
    DisplayFlashDigits(board->display, 0, 3, PERIODO_PARPADEO);


    // Crear la cola para el teclado
    colaTeclado = xQueueCreate(TAMANO_COLA_TECLADO, sizeof(evento_t));

    // Crear tarea principal
    xTaskCreate(TareaPrincipal, "TareaPrincipal", TAMANO_PILA_TAREA_PRINCIPAL, NULL, PRIORIDAD_TAREA_PRINCIPAL, &xTareaPrincipalHandle);

    // Crear tarea del teclado
    xTaskCreate(TareaTeclado, "TareaTeclado", TAMANO_PILA_TAREA_TECLADO, NULL, PRIORIDAD_TAREA_TECLADO, &xTareaTecladoHandle);

    // Crear tarea del sistema de ticks
    xTaskCreate(TareaSysTick, "TareaSysTick", TAMANO_PILA_TAREA_SYSTICK, NULL, PRIORIDAD_TAREA_SYSTICK, &xTareaSysTickHandle);

    // Crear tarea para refrescar la pantalla
    xTaskCreate(DisplayRefreshTask, "RefreshTask", TAMANO_PILA_TAREA_REFRESCO, NULL, PRIORIDAD_TAREA_REFRESCO, &xTareaRefrescoHandle);

    // Iniciar el planificador de tareas
    vTaskStartScheduler();

    // El código no debería llegar aquí
    while (1) {
    }

}

