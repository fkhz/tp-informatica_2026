#include <stdio.h>
#include <string.h>

//estructura Elemento para almacenar los datos de cada radioisopto
typedef struct Elemento{
    char Simbolo[3], Nombre[11];
    int NumeroMasico, MinutosOperacion, OperacionesTotales;
    float Rendimiento, MasaProducidaTotal, Densidad;
} Elemento;

/*
carga
inicializa el array de elementos y los totales de operacion por mes.
va sumando las operaciones, la potencia y la masa a medida que lee la producción.
 */
void CARGA(struct Elemento elementos[8], int OperacionPorMes[11], float *potenciaTotal, int *totalOperaciones){
    FILE *arch1;
    char Simbolo_1[3], Turno;
    int NumeroMasico_1, Dia, Mes, MinutosOperacion;
    float masaProducida, PotenciaOperacion;
    arch1 = fopen("produccion2025.txt", "r");
    
    FILE *arch2;
    char NombreElemento[11], Simbolo_2[3];
    int NumeroAtomico, NumeroMasico_2;
    float MasaMolar, Densidad;
    arch2 = fopen("radioisotopos.txt", "r");

    // inicializa el acumulador de minutos operados por mes en 0
    // esto sirve para el punto 1
    for(int i = 0; i < 12; i++) {
        OperacionPorMes[i] = 0;
    }

    // archivo 2
    fscanf(arch2, "%s %s %d %d %f %f", NombreElemento, Simbolo_2, &NumeroAtomico, &NumeroMasico_2, &MasaMolar, &Densidad);
    
    int i = 0;
    // inicializo el array del registro "elementos", se sabe que son 9
    while (!feof(arch2) && i < 9) {
        strcpy(elementos[i].Simbolo, Simbolo_2);
        strcpy(elementos[i].Nombre, NombreElemento);
        elementos[i].Rendimiento = 0;
        elementos[i].NumeroMasico = NumeroMasico_2;
        elementos[i].MinutosOperacion = 0;
        elementos[i].MasaProducidaTotal = 0; 
        elementos[i].OperacionesTotales = 0;
        elementos[i].Densidad = Densidad;
        
        i++;
        
        fscanf(arch2, "%s %s %d %d %f %f", NombreElemento, Simbolo_2, &NumeroAtomico, &NumeroMasico_2, &MasaMolar, &Densidad);
    }
    
    // archivo 1
    fscanf(arch1, "%s %d %f %d %d %d %f %c", Simbolo_1, &NumeroMasico_1, &masaProducida, &Dia, &Mes, &MinutosOperacion, &PotenciaOperacion, &Turno);
    
    // Bucle para procesar toda la produccion y acumular totales
    while (!feof(arch1)) {
        // acumuladores operaciones y potencia global
        // operaciones por mes para el 1, y potencia y operaciones para el 3
        OperacionPorMes[Mes-1] += MinutosOperacion;
        *potenciaTotal += PotenciaOperacion;
        *totalOperaciones += 1;
        
        int k = 0;

        // avanza mientras no haya coincidencia
        while ((strcmp(elementos[k].Simbolo, Simbolo_1) != 0) && k < 9) {
            k++;
        }

        // sale con el valor de k que coincidio
        if(k < 9){
            elementos[k].MinutosOperacion += MinutosOperacion;
            elementos[k].MasaProducidaTotal += masaProducida;
            elementos[k].OperacionesTotales++;
        }

        fscanf(arch1, "%s %d %f %d %d %d %f %c", Simbolo_1, &NumeroMasico_1, &masaProducida, &Dia, &Mes, &MinutosOperacion, &PotenciaOperacion, &Turno);
    }

    fclose(arch1);
    fclose(arch2);
}

/*
1
busca el mes con la mayor cantidad de minutos operados
pedia q sea funcion asi que segun la posicion del indice usa el return q le toque
 */
char* OPERACION_POR_MES(int OperacionPorMes[11]){
    int mins = 0;
    int pos = 0;
    
    // busca el indice del mes que tiene el mayor numero de minutos
    for (int i = 0; i < 12; i++) {
        if (OperacionPorMes[i] > mins) {
            mins = OperacionPorMes[i];
            pos = i;
        }
    }

    switch(pos){
        case 0: return "Enero"; break;
        case 1: return "Febrero"; break;
        case 2: return "Marzo"; break;
        case 3: return "Abril"; break;
        case 4: return "Mayo"; break;
        case 5: return "Junio"; break;
        case 6: return "Julio"; break;
        case 7: return "Agosto"; break;
        case 8: return "Septiembre"; break;
        case 9: return "Octubre"; break;
        case 10: return "Noviembre"; break;
        case 11: return "Diciembre"; break;
        default: return "ERROR"; break;
    }
}

/*
2
calcula el rendimiento (g/h) de cada elemento en base a su produccion
y tiempo operado, los ordena de mayor a menor y muestra el resultado
*/
void MAX_RENDIMIENTO(struct Elemento elementos[8]){
    struct Elemento aux;

    // calcular el rendimiento de cada elemento
    // rendimiento = masa total / horas totales de operacion
    for(int i = 0; i < 9; i++){
        if (elementos[i].MinutosOperacion > 0) {
            elementos[i].Rendimiento = elementos[i].MasaProducidaTotal / (elementos[i].MinutosOperacion / 60.0f); //este f es pq sino me sale q es un double y nose como arreglarlo
        }
    }
    
    // bubble
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8-i; j++){
            if(elementos[j].Rendimiento > elementos[j+1].Rendimiento){
                aux = elementos[j];
                elementos[j] = elementos[j+1];
                elementos[j+1] = aux;
            }
        }
    }

    // mostrar ordenado
    for(int i = 8; i >= 0; i--){
        printf("%s %s-%d : %.3f g/h\n", elementos[i].Nombre, elementos[i].Simbolo, elementos[i].NumeroMasico, elementos[i].Rendimiento);
    }
}

/*
3
uso la potencia total acumulada del reactor y el total de operaciones
calculo la potencia promedio y dsp el factor de desgaste
 */
void FACTOR_DE_DESGASTE(struct Elemento elementos[8], float potenciaTotal, int totalOperaciones){
    int minutosOperacionTotal = 0;
    float potenciaPromedioMW;
    float potenciaPromedioW;
    float factorDesgaste;

    // calculo el promedio en MW y dsp lo paso a W
    potenciaPromedioMW = potenciaTotal / totalOperaciones*1.0;
    potenciaPromedioW = potenciaPromedioMW * 1000000;

    // sumo todos los minutos d operacion
    for(int i = 0; i < 9; i++){
        minutosOperacionTotal += elementos[i].MinutosOperacion;
    }

    // factor de desgaste (W/min)
    factorDesgaste = potenciaPromedioW / minutosOperacionTotal*1.0;

    printf("\nPotencia promedio: %.2f MW\n", potenciaPromedioMW);
    printf("Minutos totales de operacion: %d\n", minutosOperacionTotal);
    printf("Factor de desgaste: %.2f W/min\n", factorDesgaste);
}

/*
4
se pide un simbolo quimico y se busca, si se encuentra sale del while con el ultimo valor en i
*/
void VOLUMEN(struct Elemento elementos[8], char elemento[3]){
    float volumen;
    int i = 0;
    
    // avanza mientras no encuentre el elemento o llegue a 9
    while (i < 9 && strcmp(elementos[i].Simbolo, elemento) != 0){
        i++;
    }
    
    if(i == 9){
        printf("ERROR: No se encontro ningun elemento\n");
    } else {
        volumen = elementos[i].MasaProducidaTotal / elementos[i].Densidad;
        printf("\nSe produjeron: %.2f cm3 de %s\n", volumen, elementos[i].Nombre);
    }
}

void MENU(int OperacionPorMes[11], struct Elemento elementos[8], float potenciaTotal, int totalOperaciones){
    int opt = 0;
    
    while(opt != 5) {
        printf("\n1- Máximo de horas operadas\n");
        printf("2- Listar elementos según rendimiento\n");
        printf("3- Potencia promedio\n");
        printf("4- Volumen de material producido\n");
        printf("5- Salir\n");

        printf("SU OPCIÓN: ");
        scanf("%d", &opt);

        // validacion
        while (opt < 1 || opt > 5){
            printf("ERROR: Vuelva a seleccionar una opción: ");
            scanf("%d", &opt);
        }
        
        switch (opt){
        case 1:
            printf("\nEl mes con más horas operadas es: %s\n", OPERACION_POR_MES(OperacionPorMes));
            break;
        case 2:
            printf("\n");
            MAX_RENDIMIENTO(elementos);
            break;
        case 3:
            FACTOR_DE_DESGASTE(elementos, potenciaTotal, totalOperaciones);
            break;
        case 4:
            char elemento[3];
            printf("Ingrese un simbolo quimico: ");
            scanf("%s", elemento);
            VOLUMEN(elementos, elemento);
            break;
        case 5:
            printf("Saliendo\n");
            break;
        }
    }
}

int main(){
    struct Elemento elementos[8];
    // para el 1
    int OperacionPorMes[11];
    //datos del reactor
    float potenciaTotal = 0;
    int totalOperaciones = 0; 

    CARGA(elementos, OperacionPorMes, &potenciaTotal, &totalOperaciones);
    printf("Bienvenido al menú\n");
    
    MENU(OperacionPorMes, elementos, potenciaTotal, totalOperaciones);
    
    return 0;
}