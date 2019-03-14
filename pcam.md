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

### Imágenes diseño PCAM

__Descomposición por dominio__

![Particionamiento por dominio](https://github.com/mnl359/Text-analytics/blob/master/images/domain-partitioning.png)

__Descomposición funcional__

![Particionamiento funcional](https://github.com/mnl359/Text-analytics/blob/master/images/functional-partitioning.png)

__Comunicación__

_Nota: referirse a diagramas de descomposición por dominio y funcional para entendimiento de este diagrama. Existe en este mismo GitHub una versión en la cual los datos están explícitos, si es necesario para un mejor entendimiento. Puede encontrarse en images/communication1.png_
![Comunicación (implícito)](https://github.com/mnl359/Text-analytics/blob/master/images/communication2.png)

__Aglomeración__
![Aglomeración - Envío](https://github.com/mnl359/Text-analytics/blob/master/images/agglomeration1.png)
![Aglomeración - Recepción](https://github.com/mnl359/Text-analytics/blob/master/images/agglomeration2.png)

__Mapeo__


* **P - Partitioning (Particionamiento)**
    - El particionamiento se refiere a descomponer la tarea a llevar a cabo en piezas pequeñas, de manera tal que permita identificar de manera sencilla las oportunidades que hay de ejecución paralela. 

        *Granularidad fina*: provee mayor flexibilidad para poder analizar qué partes del algoritmo pueden ser paralelizadas. 

        *Granularidad gruesa*: se refiere a la forma más compleja en la que están dados los datos. 

        La descomposición de datos se puede hacer en diferentes fases. 
        Lo ideal es que en primera instancia se dividan los datos de mayor volumen o a los que más se acceden (granularidad gruesa) en datos más pequeños de más o menos el mismo tamaño (granularidad fina). 

        - Hay una parte de descomposición por dominio (es decir, los datos).

        - Hay una parte de descomposición funcional (referente al procesamiento).
    
      * Los datos que se tienen para este problema son, desde los más robustos hasta los más atómicos:
        - Directorio -> donde están almacenados los ficheros a ser leídos. 
        - Fichero -> estructura que acumula las noticias en las cuales será buscada la palabra que el usuario     requiere encontrar. 
        - Noticia -> compuesta por id, título, editor, autor, fecha, año, mes, url, contenido. 
        - Arreglo de noticias -> guarda el resultado final que se mostrará en pantalla.
        - Arreglo de noticias en posición i (i = 0..n-1) -> almacena en una estructura <contador, id, título>.    Dos de esos elementos se extraen de la estructura "Noticia".
        - Palabra -> 
            * Palabra buscada: la palabra cuya frecuencia se quiere conocer. 
            * Palabra: que compone al contenido.
        - Contador -> variable que almacena la cantidad de veces que la palabra buscada ocurre en el contenido de     una noticia. 
    
      * Las tareas son explicadas en el diagrama de descomposición funcional.

* **C - Communication (Comunicación)**
    - La comunicación se refiere al proceso de intercambio de datos entre tareas. Esto se debe a que muchas veces para que una tarea pueda ejecutarse, necesita datos que le debe entregar otra tarea, así que debe haber un canal de comunicación entre ellas. 

    Para el problema tratado en esta activida, en general la comunicación entre las diferentes tareas es:
      - Local: porque cada tarea se comunica con un conjunto de otras tareas, sin que haya centralización de una tarea en particular.

      - No estructurada: ya que al crear las redes de comunicación, se obtuvo un grafo arbitrario.

      - Estática: puesto que la volatilidad de los datos no es suficiente como para cambiar la identidad de la comunicación en ejecución.

      - Síncrona con buffer. 

    *¿Cómo se va a distribuir la comunicación y la computación?*: 

* **A - Agglomeration (Aglomeración)**
    - Se trata de la manera de combinar las tareas que se identificaron en la fase de particionamiento para poder aumentar la granularidad de la solución y determinar si es o no necesario replicar datos y computación.

    Para la solución actual se realizó aglomeración por computación. Esto debido a que las tareas se pueden reducir si se tiene en cuenta que las funcionalidades serán llevadas a cabo por varias máquinas (nodos). 
    
      - Cada fichero puede ser distribuido a un nodo diferente para que se encargue de procesarlo.
      - Buscar la palabra en el contenido es una tarea que puede ser llevada a cabo paralelamente por hilos.
      - Retornar el contador y el identificador y título de la noticia sería la finalidad de cada hilo.
      - Devolver el arreglo sería una tarea de cada uno de los nodos que participa con el procesamiento de un fichero.
      - Recibir al final todos los datos y hacer el 'reduce' final de estos se le asigna al nodo controlador.
    
    De esta manera, las tareas llevadas a cabo de forma secuencial cada vez que el algoritmo entraba en un ciclo, ahora se pueden realizar al mismo tiempo por cada una de las máquinas y sus respectivos hilos. 

    También se logró reducir la cantidad de veces que un dato era llevado entre tareas. 
    El arreglo final que guarda los datos necesarios para mostrar en pantalla sobre las noticias, pasa a través de menos tareas, igual que la palabra a ser buscada. Dichas estructuras eran solicitadas en varios de los procesos pero se disminuyó, en cada nodo, la cantidad de veces pasados entre actividades. 