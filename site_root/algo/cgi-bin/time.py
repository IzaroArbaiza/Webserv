import time

# Obtener la hora actual en formato de 24 horas
hora_actual = time.strftime("%H:%M:%S")

# Limpiar la pantalla (en sistemas Unix como Linux/MacOS)
print("\033c", end="")  # Para Windows, usa 'cls' en lugar de '\033c'

# Mostrar la hora actual
print(hora_actual)
