# DISEÑO DE ALGORTIMO DISTRIBUIDO PARALELO

## MÉTODO PCAM

### Algoritmo en serial
    
    pedir palabra a ser buscada
    abrir directorio
    crear arreglo_noticias vacío
    por cada fichero en directorio
        abrir fichero
        definir delimitadores fichero
        por cada noticia en fichero
            por cada palabra en noticia
                si palabra_actual == palabra_buscada:
                    aumentar contador
                fin si
            fin ciclo
            si contador > 0
                por cada posición de arreglo_noticias
                    si contador > arreglo_noticias[posición]
                        guardar noticia y contador en arreglo_noticias[posición]
                    fin si
                fin ciclo
            fin si
        fin ciclo
        cerrar fichero    
    fin ciclo
    imprimir arreglo_noticias
    cerrar directorio

* **P - Partitioning (Particionamiento)**
    - El particionamiento se refiere a descomponer la tarea a llevar a cabo en piezas pequeñas, de manera tal que permita identificar de manera sencilla las oportunidades que hay de ejecución paralela. 

        *Granularidad fina*: provee mayor flexibilidad para poder analizar qué partes del algoritmo pueden ser paralelizadas. 

        *Granularidad gruesa*: se refiere a la forma más compleja en la que están dados los datos. 

        La descomposición de datos se puede hacer en diferentes fases. 
        Lo ideal es que en primera instancia se dividan los datos de mayor volumen o a los que más se acceden (granularidad gruesa) en datos más pequeños de más o menos el mismo tamaño (granularidad fina). 

        - Hay una parte de descomposición por dominio (es decir, los datos).

        - Hay una parte de descomposición funcional (referente al procesamiento).


### Imágenes diseño PCAM

__Descomposición por dominio__

![Particionamiento por dominio](https://github.com/mnl359/Text-analytics/blob/master/domain-partitioning.png)

__Descomposición funcional__

![Particionamiento funcional](https://github.com/mnl359/Text-analytics/blob/master/functional-partitioning.png)

__Comunicación__

![Comunicación](https://github.com/mnl359/Text-analytics/blob/master/communication.png)