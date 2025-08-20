import sys

class Automata:
    def __init__(self, estados, alfabeto, transiciones, estado_inicial, estados_finales):
        self.estados = estados
        self.alfabeto = alfabeto
        
        # Validar origen de transiciones
        if not all(origen in estados for (origen, letra) in transiciones.keys()):
            print("Las transiciones deben estar con estados validos")
            sys.exit()
            
        if not all(destino in estados for destino in transiciones.values()):
            print("Las transiciones deben estar con destinos validos")
            sys.exit()
        self.transiciones = transiciones

        if estado_inicial not in estados:
            print("El estado inicial no está en el conjunto")
            sys.exit()

        self.estado_inicial = estado_inicial

        if not all(estado in estados for estado in estados_finales):
            print("Los estados finales no están en el conjunto")
            sys.exit()

        self.estados_finales = estados_finales

    def acepta(self, cadena):
        estado_actual = self.estado_inicial
        for simbolo in cadena:
            if (estado_actual, simbolo) not in self.transiciones:
                print(f"No hay transicion definida para ({estado_actual}, {simbolo})")
                return False
            estado_actual = self.transiciones[(estado_actual, simbolo)]

        return estado_actual in self.estados_finales

def main():
    if len(sys.argv) != 3:
        print("Hace falta uno o mas archivos ")
        return

    caracteres = str.maketrans("", "", "{}\n")
    nombre_archivo = sys.argv[1]
    archivo_cadenas = sys.argv[2]

    try:
        with open(nombre_archivo, "r") as f:
            lineas = f.readlines() 
       
            if len(lineas) != 5:
                print("El archivo debe tener 5 lineas")
                return

            estados = lineas[0].translate(caracteres).split(",")
            alfabeto = lineas[1].translate(caracteres).split(",")
            estados = [e.strip() for e in lineas[0].translate(caracteres).split(",")]
            alfabeto = [a.strip() for a in lineas[1].translate(caracteres).split(",")]

            transiciones_raw = lineas[2].translate(caracteres).split(",")
            transiciones = {}
            for t in transiciones_raw:
                if not t.strip():
                    continue

                izquierda, destino = t.split("=", 1)
                izquierda = izquierda.strip("()") 

                izquierda = izquierda.split(":")

                transiciones[(izquierda[0], izquierda[1])] = destino

            estado_inicial = lineas[3].translate(caracteres).strip()
            estados_finales = lineas[4].translate(caracteres).split(",")
            automata = Automata(estados, alfabeto, transiciones, estado_inicial, estados_finales)
            print("Automata creado correctamente")
            print("Estados:", automata.estados)
            print("Alfabeto:",automata.alfabeto)
            print("Transiciones:", automata.transiciones)
            print("Estado inicial:", automata.estado_inicial)
            print("Estados finales:", automata.estados_finales)
            

            with open(archivo_cadenas, "r") as f:
                cadenas = [linea.strip() for linea in f.readlines() if linea.strip()]
            for cadena in cadenas:
                resultado = automata.acepta(cadena)
                print('ACEPTADA' if resultado else 'RECHAZADA')

    except FileNotFoundError:
        print("Archivo no encontrado.")
    except Exception as e:
        print(f"Ocurrio un error: {e}")

if __name__ == "__main__":
    main()
