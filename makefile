BOARD ?= edu-ciaa-nxp
##<! Variable donde indico la board
MUJU ?=../muju
##<! Variable donde coloco la direccion de muju

MODULES := module/freertos

include $(MUJU)/module/base/makefile ##<! Incluyo el makefile de MUJU


## Todas las reglas que usaremos vienen de MUJU