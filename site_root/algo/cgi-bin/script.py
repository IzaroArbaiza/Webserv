import random
import time

def buscar_baguette():
    print("Pierre sale de su casa con una misión: encontrar la baguette perfecta.")
    time.sleep(2)

    lugares = ["panadería", "supermercado", "parque", "debajo de la Torre Eiffel", "en casa de su vecino Jean"]
    encontrado = random.choice([True, False])

    for lugar in lugares:
        print(f"Buscando en {lugar}...")
        time.sleep(1.5)
        
        if encontrado and random.random() > 0.5:
            print(f"¡Sacré bleu! ¡Pierre ha encontrado la baguette en {lugar}! Vive la France!")
            return
    
    print("Pierre no encuentra ninguna baguette... ¡Esto es un escándalo nacional!")
    print("Pierre protesta en la calle. ¡La revolución comienza!")

if __name__ == "__main__":
    buscar_baguette()
