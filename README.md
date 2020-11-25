# DMUTEX

La práctica consiste en el desarrollo de un prototipo que implementa la lógica de control para la gestión de relojes lógicos vectoriales así como el protocolo de acceso a una región con exclusión mutua. Para ello se deberá desarrollar un programa que:
- Mediante la entrada estándar (``stdin``) reciba la secuencia de acciones a realizar.
- Utilice el protocolo UDP y la librería de _sockets_ para comunicarse con otros procesos análogos.
- Genere mensajes de traza y control por medio de la salida estándar (``stdout``).
El programa solicitado solo tendrá que tener un único hilo de ejecución (no es necesario implementar varios _threads_) puesto que la entrada de acciones será la que determinará la secuencia de sucesos que atenderá el proceso. Se ha de entender que esto es una implementación de un prototipo y que este mecanismo permite forzar secuencias de acciones que permitan estudiar el comportamiento de ambos algortimos (es una simulación).
