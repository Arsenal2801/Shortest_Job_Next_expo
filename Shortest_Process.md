# El proceso mas corto a continución

## ¿Qué es?

Es un algoritmo que se implementa en sistemas interactivos. Estos algoritmos son comunes en PC, Servidores y otros sistemas.

## ¿Cómo funcionó?

Este algoritmo al igual que el algoritmo de tipo el mas corto primero siempre produce el tiempo de respuesta promedio minimo para los sistemas de procesamiento por lotes. Por lo general los procesos inactivos tienden a seguir un patron de esperar un comando y ejecutarlo, esperar, ejecutar...

Si consideramos la ejecucion de un comando como un "trabajo" separado entonces podriamos minimizar el el tiempo de respuestaa total mediante la ejecucion del mas corto primero. Con respecto a esto se present un problema ¿Cómo sabemos cual es el mas corto?

Un metodo es la realizacion de estimaciones con base al comportamiento anterior y ejecutar el proceso  con el menor tiempo estimado.

Suponga que el tiempo estimado por cada comando para cierta terminal es $T_{0}$. Ahora supongaa que su siguiente ejecucion se mide como $T_{1}$. Podriamos actualizar nuestra estimacion mediante un suma ponderada de estos numeros, es decir, $aT_{0}+(1-a)T_{1}$. Por medio de la eleccion de a podemos decidir hcer que el proceso de estimacion olvide las ejecuciones anteriores rapidamente o que sean recordaadas por mucho tiempo. Con $a=1/2$ obtenemos sucesiones estimadas de:
	$$T_{0}, T_{0}/2 + T_{1}/2, T_{0}/4 + T_{1}/4 + T_{2}/2, T_{0}/8 + T_{1}/8 + T_{2}/4 + T_{3}/2$$

Despues de tres nuevas ejecuciones el peso de $T_{0}$ se vuelve de $1/8$.

La tecnica de estimar el siguiente valor en una serie mediante la obtencion del promedio ponderado del valor actual medido y la estimacion anterior se conoce algunas veces como ***envejecimiento***.

Se aplica en muchas situaciones en donde se debe realizar una prediccion con base en valores anteriores. En envejecimiento es facil de implementaar cuando $a=1/2$. Todo lo que se necesita es sumar el nuevo valor a la estimacion actual y dividir la suma entre 2 mediante desplazamiento a la derecha.



## Investigacion

La planificación de procesos, específicamente referida al algoritmo del "proceso más corto siguiente" (Shortest Job Next o SJN), es un método de planificación de CPU donde el proceso con la menor ráfaga de CPU pendiente se selecciona para ejecutarse primero. Esto significa que si hay varios procesos en cola esperando ser ejecutados, el proceso con el tiempo de ejecución más corto se ejecutará antes que los demás.

Aquí hay una descripción básica de cómo funciona el algoritmo SJN:

1. **Llegada de procesos**: Los procesos llegan a la cola de listos en momentos diferentes. Cada proceso tiene un tiempo estimado de ejecución.

2. **Selección del proceso más corto**: El planificador de la CPU selecciona el proceso con la ráfaga de CPU más corta en la cola de listos para ejecutarse a continuación.

3. **Ejecución del proceso seleccionado**: El proceso seleccionado se ejecuta hasta que se completa su ráfaga de CPU o hasta que se interrumpe por algún evento, como una solicitud de E/S.

4. **Actualización de la cola de listos**: Después de que un proceso ha sido ejecutado, se actualiza la cola de listos y se repite el proceso de selección del proceso más corto entre los procesos restantes en cola.

5. **Finalización del proceso**: Los procesos continúan siendo seleccionados y ejecutados hasta que todos hayan sido completados.

El algoritmo SJN es eficiente en términos de minimizar el tiempo promedio de espera de los procesos en la cola, ya que siempre selecciona el proceso más corto en cada paso. Sin embargo, puede sufrir de inanición si procesos cortos continúan llegando y nunca permiten que los procesos más largos se ejecuten.

En la práctica, la implementación real del algoritmo SJN puede requerir consideraciones adicionales, como la predicción precisa del tiempo de CPU de cada proceso y cómo manejar situaciones donde dos procesos tienen la misma ráfaga de CPU más corta.



## Notas expo:
### Introducción:

El algoritmo SPN (Shortest Process Next) es un algoritmo de planificación de procesos no preventivo que selecciona para su ejecución el proceso de la cola de listos con el menor tiempo de ráfaga restante. El objetivo de este algoritmo es minimizar el tiempo promedio de espera y el tiempo promedio de respuesta de los procesos.

### Ventajas:

Menor tiempo promedio de espera: SPN tiende a minimizar el tiempo promedio de espera de los procesos, ya que los procesos cortos se ejecutan antes que los largos.
Menor tiempo promedio de respuesta: SPN también tiende a minimizar el tiempo promedio de respuesta de los procesos, ya que los procesos reciben CPU lo antes posible.
Simpleza: El algoritmo SPN es relativamente simple de implementar.

### Desventajas:

No es preventivo: SPN no es un algoritmo preventivo, lo que significa que no puede interrumpir la ejecución de un proceso para ejecutar otro proceso con mayor prioridad. Esto puede dar lugar a que los procesos largos monopolicen la CPU y perjudiquen a los procesos cortos.
Requiere conocimiento previo del tiempo de ráfaga: Para que SPN funcione de manera eficiente, es necesario conocer el tiempo de ráfaga de cada proceso antes de que llegue al sistema. Esto puede ser difícil o imposible de obtener en algunos casos.
Ineficiente para ráfagas bursty: SPN puede ser ineficiente para procesos con ráfagas bursty, es decir, procesos que alternan entre períodos de uso intensivo de CPU y períodos de inactividad. Esto se debe a que el algoritmo puede seleccionar un proceso corto para su ejecución, pero este proceso puede tener una ráfaga larga, lo que retrasa la ejecución de otros procesos.

### Consideraciones de diseño para la implementación de SPN:

Elección de la estructura de datos para la cola de listos: La elección de la estructura de datos adecuada para la cola de listos es crucial para la eficiencia del algoritmo SPN. Una cola de prioridad o una lista ordenada son opciones comunes, ya que permiten acceder rápidamente al proceso con el menor tiempo de ráfaga restante.
Manejo de interrupciones: Si se implementa SPN en un sistema operativo que admite interrupciones, es necesario definir cómo se manejarán las interrupciones durante la ejecución de un proceso. Una opción es reubicar el proceso interrumpido al final de la cola de listos, mientras que otra opción es darle prioridad a la ejecución del proceso interrumpido.
Estimación del tiempo de ráfaga: Si no es posible conocer el tiempo de ráfaga exacto de un proceso antes de su llegada, se pueden utilizar técnicas de estimación para obtener una aproximación. Existen diferentes técnicas de estimación, como la estimación basada en el historial del proceso o la estimación basada en el tipo de proceso.
Manejo de procesos de larga duración: Para evitar que los procesos de larga duración monopolicen la CPU, se pueden implementar mecanismos como el envejecimiento o la multiprogramación con prioridades. El envejecimiento aumenta la prioridad de los procesos que han estado esperando en la cola de listos durante un tiempo prolongado, mientras que la multiprogramación con prioridades permite ejecutar simultáneamente varios procesos con diferentes prioridades.

### Conclusión:

El algoritmo SPN es una opción viable para la planificación de procesos en sistemas donde el objetivo principal es minimizar el tiempo promedio de espera y el tiempo promedio de respuesta. Sin embargo, es importante considerar las limitaciones del algoritmo y tomar las medidas adecuadas para mitigarlas al momento de implementarlo.

### Recursos adicionales:

https://en.wikipedia.org/wiki/Shortest_job_next
https://www.geeksforgeeks.org/cpu-scheduling-in-operating-systems/
https://www.tutorialspoint.com/operating_system/pdf/os_process_scheduling_algor
