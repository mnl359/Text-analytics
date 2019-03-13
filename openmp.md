# Minería de texto con OpenMP y MPI

## Problema

Se tiene un conjunto de noticias en texto libre, sobre el cual se desea calcular:

Índice Invertido, donde por cada palabra que se ingrese por teclado, se liste en orden descendente por frecuencia de palabra en el contenido <content> de la noticia, las noticias más relevantes. Listar máx 10 <frec,doc_id,title>.

Los datos de trabajo para el proyecto son un conjunto de noticias publicados en kaggle:
    https://www.kaggle.com/snapcrack/all-the-news

## Solución planteada con paralelismo.

**Uso de framework OpenMP con C++**

  1. Pedir ruta a ficheros que se van a leer. 
  2. Pedir palabra a ser buscada.
  3. Abrir el directorio desde el que se van a leer los ficheros. 
  4. Iterar sobre cada uno de los ficheros.
  5. Abrir el fichero.
  6. Definir delimitadores para saber cuáles son palabras, cuándo hay un fin de línea y cuando es el fin del fichero.
  7. Leer cada uno de los caracteres del fichero. 
  8. Utilizar hilos para cada una de las líneas del fichero. 
  9. Aumentar la variable contador, compartida entre los hilos, cada vez que se encuentre la palabra buscada.
  10. Repetir los puntos 6 a 8 hasta encontrar el fin del fichero.
  11. Cerrar el fichero.
  12. Retomar el paso 1 en caso que haya más ficheros en el directorio que deben ser leídos. 
  13. Organizar, de acuerdo al índice invertido, los datos calculados. 
  14. Imprimir los datos. 

## Solución planteada de forma distribuida con paralelismo. 

**Uso de los frameworks OpenMP y MPI con C++**

**Nota:** en esta solución, se utilizará un nodo Master y dos nodos esclavos provistos desde el DCA de la universidad EAFIT para estudiantes que cursan la materia Tópicos Especiales en Telemática en el semestre 2019-1.

  1. Pedir ruta a ficheros que serán leídos. 
  2. Pedir palabra a ser buscada.
  3. Distribuir ficheros desde el master a cada uno de los esclavos.
  4. En los esclavos:
      1. Abrir el fichero.
      2. Definir delimitadores para saber cuáles son palabras, cuándo hay un fin de línea y cuando es el fin del fichero.
      3. Leer cada uno de los caracteres del fichero. 
      4. Utilizar hilos para cada una de las líneas del fichero. 
      5. Aumentar la variable contador, compartida entre los hilos, cada vez que se encuentre la palabra buscada. 
      6. Repetir los puntos 3 a 5 hasta encontrar el fin del fichero.
      7. Cerrar el fichero.
      8. Guardar en buffer el contador final.
      9. Repetir los pasos 1 a 8 de acuerdo a la cantidad de ficheros que se le hayan asignado. 
  5. En el master:
      1. Una vez el conteo esté hecho en cada esclavo, recibir los resultados de cada uno de estos. 
      2. Organizar, de acuerdo al índice invertido, los datos que le fueron retornados. 
      3. Imprimir los datos. 
  