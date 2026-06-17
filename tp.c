#include <stdio.h>

void PANTALLA(int *opt){
    printf("\n1- Maximo de horas operadas\n");
    printf("2- Mostrar rendimiento\n");
    printf("3- Potencia promedio\n");
    printf("4- Volumen de material producido\n");
    printf("5- Salir\n");

    printf("SU OPCIÓN: ");

    scanf("%d", opt);

    while (*opt < 0 || *opt > 5)
    {
        printf("ERROR: Vuelva a seleccionar una opci�n: ");
        scanf("%d", opt);
    }
}

void CARGA(){
    FILE *datos;
    int codigoElemento, numeroMasico, dia, mes, minutosOperacion;
    float masaProducida, potenciaOperacion;
    char turno;
    datos = fopen("produccion2025_etapa1.txt", "r");

    int N[11];


    fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
    //printf("%d,%d,%f,%d,%d,%d,%f,%c\n", codigoElemento, numeroMasico, masaProducida, dia, mes, minutosOperacion, potenciaOperacion, turno);

    for(int i = 0; i<13; i++){
        N[i]=0;
    }
    while (!feof(datos)) {
        if(N[mes-1] < minutosOperacion){
            N[mes-1] = minutosOperacion;


        }









        fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
        //printf("%d,%d,%f,%d,%d,%d,%f,%c\n", codigoElemento, numeroMasico, masaProducida, dia, mes, minutosOperacion, potenciaOperacion, turno);
        }
        for(int j= 0; j<12;j++){
            printf("mes:%d | %d\n",j, N[j]);
            }
    fclose(datos);


}
int OPERACION_POR_MES(int inMes){
    FILE *datos;
    int codigoElemento, numeroMasico, dia, mes, minutosOperacion;
    float masaProducida, potenciaOperacion;
    char turno;
    datos = fopen("produccion2025_etapa1.txt", "r");

    int outmins = 0;

    fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);

    while (!feof(datos)) {
        if (inMes == mes)
        {
            if (minutosOperacion > outmins)
            {
                outmins = minutosOperacion;
            }
        }
        fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
    }
    fclose(datos);

    return (outmins);
}

float RENDIMIENTO(int inCodigoElemento){
    FILE *datos;
    int codigoElemento, numeroMasico, dia, mes, minutosOperacion;
    float masaProducida, potenciaOperacion;
    char turno;
    datos = fopen("produccion2025_etapa1.txt", "r");

    float outRendmiento;
    float masaProducidaTotal = 0;
    int minutosOperacionTotal = 0;

    fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);

    while (!feof(datos)) {
        if (inCodigoElemento == codigoElemento)
        {
            masaProducidaTotal = masaProducidaTotal+masaProducida;
            minutosOperacionTotal = minutosOperacionTotal+minutosOperacion;
        }
        fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
    }
    fclose(datos);

    outRendmiento = masaProducidaTotal/(float)minutosOperacionTotal;

    return (outRendmiento);
}

void FACTOR_DE_DESGASTE(float *potenciaPromedio, int *minutosOperacionTotal, float *factorDesgaste){
    FILE *datos;
    int codigoElemento, numeroMasico, dia, mes, minutosOperacion;
    float masaProducida, potenciaOperacion;
    char turno;
    datos = fopen("produccion2025_etapa1.txt", "r");

    fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
    printf("%d,%d,%f,%d,%d,%d,%f,%c\n", codigoElemento, numeroMasico, masaProducida, dia, mes, minutosOperacion, potenciaOperacion, turno);
    int contador = 1;
    float potenciaTotal = 0;

    while (!feof(datos)) {
        potenciaTotal = potenciaTotal + potenciaOperacion;
        *minutosOperacionTotal = *minutosOperacionTotal + minutosOperacion;
        contador++;

        fscanf(datos, "%d,%d,%f,%d,%d,%d,%f,%c", &codigoElemento, &numeroMasico, &masaProducida, &dia, &mes, &minutosOperacion, &potenciaOperacion, &turno);
        printf("%d,%d,%f,%d,%d,%d,%f,%c\n", codigoElemento, numeroMasico, masaProducida, dia, mes, minutosOperacion, potenciaOperacion, turno);
    }
    fclose(datos);

    *potenciaPromedio = potenciaTotal/(float)contador;
    *factorDesgaste = *potenciaPromedio*1000000/(float)*minutosOperacionTotal;

    printf("\nPotencia promedio: %.1f MW\n", *potenciaPromedio);
    printf("Minutos de operación: %d\n", *minutosOperacionTotal);
    printf("Factor de desgaste: %f W/min\n", *factorDesgaste);
}

int main(){
    CARGA();
    int opt;
    printf("Bienvenido al menú");
    PANTALLA(&opt);

    while (opt != 0 && opt != 5) {
        switch (opt){
        case 1:
            int inMes;
            printf("\nIngrese un mes: ");
            scanf("%d", &inMes);
            while (inMes < 1 || inMes > 12){
            printf("ERROR: Vuelva a ingresar un mes: ");
                scanf("%d", &inMes);
            }
            printf("\nLa cantidad de minutos es: %d\n", OPERACION_POR_MES(inMes));
            break;
        case 2:
            int inCodigoElemento;
            printf("\nIngrese un código de elemento: ");
            scanf("%d", &inCodigoElemento);
            while (inCodigoElemento < 1 || inCodigoElemento > 9){
            printf("ERROR: Vuelva a ingresar un elemento: ");
                scanf("%d", &inCodigoElemento);
            }
            printf("\nEl rendimiento es: %.3f\n", RENDIMIENTO(inCodigoElemento));
            break;
        case 3:
            float potenciaPromedio = 0;
            int minutosOperacionTotal = 0;
            float factorDesgaste = 0;
            FACTOR_DE_DESGASTE(&potenciaPromedio, &minutosOperacionTotal, &factorDesgaste);
            break;
        case 4:
            printf("\nSe seleccionó la opción 4\n");
            break;
        }
        PANTALLA(&opt);
    }
    printf("\nPrograma finalizado.\n");
    return (0);
}
