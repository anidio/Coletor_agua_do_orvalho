Python 3.9.6 (tags/v3.9.6:db3ff76, Jun 28 2021, 15:26:21) [MSC v.1929 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license()" for more information.
>>> import serial
conn = serial.Serial('COM4', 9600)

def salvar():
        f2= open ('ms1.txt', 'a')
        dados = conn.readline().decode('utf-8').replace ('.',',')         
        print(conn.readline().decode('utf-8').replace ('.',','))
        print ("LOCAL: Suape")
        f2.write(conn.readline().decode('utf-8').replace ('.',','))
        
def main():
        print("[!] Status port: %s" %conn.isOpen() )
        print("[!] Port Name: %s" %conn.name)
        
        criando = open('ms1.txt', 'a')
        criando.write("")
        criando.close()
        
        verificar = open('ms1.txt','r')
        l1 = verificar.readline()
        verificar.close()
        
        if l1 != "LOCAL: Suape\n":
                f1 = open('ms1.txt', 'a')
                f1.write("LOCAL: Suape\n")
                f1.write("Data Hora Umid.Ar Temp.Ar Temp.Orv. Temp.placa fria \n")
                f1.close()

                while True:
                        salvar()
                        
        else:
                while True:
                        salvar()
          
main()
