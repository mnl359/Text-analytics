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

        - Hay una parte de descomposición por dominio (es decir, los datos).


        - Hay una parte de descomposición funcional (referente al procesamiento).


![Diseño PCAM](https://github.com/mnl359/Text-analytics/blob/master/pcam.png)