AGG_posicion = list()


tiempo_AGG_posicion = list()


fichero = 'AM3.txt'

with open(fichero) as f:
    for linea in f:
        # Tu código aquí
        for i in linea.split():
            if i=="AGG-posicion":
                AGG_posicion.append(linea.split())
            elif i=="Tiempo_AGG-p":
                tiempo_AGG_posicion.append(linea.split())


cadGeneracional = "".join( [str(_) for _ in AGG_posicion] )
cadGeneracional = cadGeneracional.replace("[", "")
cadGeneracional = cadGeneracional.replace("]", "")
cadGeneracional = cadGeneracional.replace("'", "")
cadGeneracional = cadGeneracional.replace(",", "")
cadGeneracional = cadGeneracional.replace("AGG-posicion", "")
cadGeneracional = cadGeneracional.replace(" ", "\n")



print("AGG-posicion")
print(cadGeneracional)

print("\n")

tiempo = "".join( [str(_) for _ in tiempo_AGG_posicion] )
tiempo = tiempo.replace("[", "")
tiempo = tiempo.replace("]", "")
tiempo = tiempo.replace("'", "")
tiempo = tiempo.replace(",", "")
tiempo = tiempo.replace("Tiempo_AGG-p", "")
tiempo = tiempo.replace(" ", "\n")



print("tiempo_AGG_posicion")
print(tiempo)

print("\n")