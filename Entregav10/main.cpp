#include <iostream>
#include <queue>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

int cont_cuadruplos;
string signo_aux;
string operandos_aux[2];
vector<string> temp_aux;
vector<string> cuadruplo_aux;
stack<string> openParen;

int pos_vector;
bool bandera=false;
//Se guarda el nombre del último temporal generado en el cuádruplo.
//Sirve para saber en dónde agregar el paréntesis que cierra.
string tempCierre;
string parentesis=""; //Guarda todos los paréntesis encontrados
string expFinal="";

//Para el archivo
vector<string> cuadruplos;
ifstream inF;
ofstream outF;
string inS,nameFile;
bool keyFunc=false,forflag=false, ifflag=false, whileflag=false, elseflag=false;
int cont=0; //Para revisar si ya sordeó un key dentro de una funcion

void cierre_parentesis(string temp_anterior)
{
    int position=0;
    for (int i=0; i<temp_aux.size(); i++)
    {
        if (temp_aux[i]==temp_anterior)
            position=i;
    }
    cuadruplo_aux[position]=cuadruplo_aux[position]+")";
}

int temp_find(string temporal)
{
    int position=0;
    for (int i=0; i<temp_aux.size(); i++)
    {
        if (temp_aux[i]==temporal)
            position=i;
    }
    return position;
}

string generateExpression()
{
    /***
    *Para generar las expresiones!
    **/
    string in;
    for(int i=0; i<cuadruplos.size();i++)
    {
        in=cuadruplos[i];
            //El cuadruplo es un signo
            if (in=="*"||in=="/"||in=="%"||in=="+"||in=="-"||in=="<"||in==">"||in=="=="||in=="!="||in=="="||in=="&&"||in=="||")
            {
                cont_cuadruplos=0;
                signo_aux=in;
            }

            //El cuádruplo es un temporal para guardar resultado
            if (cont_cuadruplos==3 && in[0]=='T')
            {
                cuadruplo_aux.push_back(operandos_aux[0]+signo_aux+operandos_aux[1]);
                temp_aux.push_back(in);
                cont_cuadruplos=0;
                tempCierre=in; //Guardas el último temporal procesado
            }

            //El cuádruplo es un temporal en la posición 1
            if (cont_cuadruplos==1 && in[0]=='T')
            {
                pos_vector=temp_find(in);
                if (!openParen.empty())
                {
                    while (!openParen.empty())
                    {
                        parentesis=openParen.top()+parentesis;
                        openParen.pop();
                    }
                    operandos_aux[0]=parentesis+cuadruplo_aux[pos_vector];
                    parentesis="";

                }
                else
                    operandos_aux[0]=cuadruplo_aux[pos_vector];
                bandera=true;
            }

            //El cuádruplo es un temporal en la posición 2
            if (cont_cuadruplos==2 && in[0]=='T')
            {
                pos_vector=temp_find(in);
                operandos_aux[1]=cuadruplo_aux[pos_vector];
                bandera=true;
            }

            //El cuádruplo tiene el signo de igualdad y se hará la asignación en la posición 2
            if ((cont_cuadruplos==2 && signo_aux=="=") )
            {
                expFinal=in+signo_aux+operandos_aux[0];
                cuadruplos.clear();
                return expFinal;
                //cout << "La expresión final queda " << inS << signo_aux << operandos_aux[0]<< endl;
                cont_cuadruplos=0;
            }

            //El cuádruplo es un número
            if (bandera==false)
            {
                if (cont_cuadruplos==1)
                {
                    if (!openParen.empty())
                    {
                        while (!openParen.empty())
                        {
                            parentesis=openParen.top()+parentesis;
                            openParen.pop();
                        }

                        operandos_aux[0]=parentesis+in;
                        parentesis="";
                    }
                    else
                        operandos_aux[0]=in;
                }
                if (cont_cuadruplos==2)
                    operandos_aux[1]=in;
            }

            if (in=="(")
                openParen.push("(");

            if (in==")")
                cierre_parentesis(tempCierre);

            cont_cuadruplos++;
            bandera=false;

            ///Termina generación de expresiones!
    }

        expFinal=operandos_aux[0]+signo_aux+operandos_aux[1];
        cuadruplos.clear();
    return expFinal;

}

/***
*This function converts the reserved words DO,RE,MI,FA,SOL,LA,SI,DOP,REP,MIP,FAP,SOLP,LAP,SIP
*into the frecuency number that corresponds for each one. Example LA = 440Hz
*Reference: http://plasticity.szynalski.com/tone-generator.htm
*/
string convertToTone(string tone)
{
    if(tone=="DO") return "523.25";
    if(tone=="RE") return "587.32";
    if(tone=="MI") return "659.25";
    if(tone=="FA") return "698.45";
    if(tone=="SOL") return "783.99";
    if(tone=="LA") return "880";
    if(tone=="SI") return "987.76";
    if(tone=="DOP") return "1046.5";
    if(tone=="REP") return "1174.65";
    if(tone=="MIP") return "1318.51";
    if(tone=="FAP") return "1396.91";
    if(tone=="SOLP") return "1567.98";
    if(tone=="LAP") return "1660";
    if(tone=="SIP") return "1975.52";
}
int main()
{
    cin >> nameFile;    //Nombre del programa final a generar
    inF.open(nameFile.c_str());     //Lee desde el archivo de entrada asignado
    nameFile+=".ino";
    outF.open(nameFile.c_str());    //abre un archivo de salida con el mismo nombre de entrada

    outF << "#include \"Robotec.h\""<<endl<<endl
         << "Robotec robot;" <<endl
         <<"float distance,temp, lightR, lightL;\n"
         << "void setup()" <<endl
         << "{"<<endl
         <<    "\tSerial.begin(9600);\n"
         <<    "\tpinMode(2, OUTPUT);\n"
         <<    "\tpinMode(4, OUTPUT);\n"
         <<    "\tpinMode(6, OUTPUT);\n"
         <<    "\tpinMode(7, OUTPUT);\n"
         <<    "\tpinMode(8, OUTPUT);\n"
         <<    "\tpinMode(9, OUTPUT);\n"
         <<    "\tpinMode(10, OUTPUT);\n"
         <<    "\tpinMode(11, OUTPUT);\n"
         <<    "\tpinMode(12, OUTPUT);\n"
         <<    "\tpinMode(13, OUTPUT);\n"
         <<    "\trobot.MoveRoboTime(0,0,0);"<<endl
         <<"}"<<endl;

    while(inF >> inS)
    {
        //generateExpression();

        if(inS == "mainStart")
        {
            outF<<"\nvoid loop()"<<endl;

        }else if(inS == "mainEnd")
        {
            outF<<"\n"<<endl;

        }else if(inS == "void")//cada vez que encuentre una función
        {
            outF << "\nvoid "; inF>>inS; outF<<inS;
        }else if(inS == "startParam")//Cada vez que inicien los parámetros de la función
        {
            outF <<"(";
            string myParams,myParams2;

            inF>>myParams>>myParams2;
            if(myParams=="endParam")
            {
                if(myParams2=="string")
                        myParams2 = "String";
                if(myParams=="string")
                    myParams = "String";
                if(myParams2!="startBlock")
                    outF<<"){\n"<<myParams2<<" ";
                else
                    outF<<"){\n"<<" ";
            }
            else
            {
                outF <<" "<< myParams << " "<<myParams2;
                while(inF>>myParams>>myParams2 && myParams2 != "endParam" && myParams != "endParam")
                {
                    if(myParams2=="string")
                        myParams2 = "String";
                    if(myParams=="string")
                        myParams = "String";
                    outF <<" "<< myParams << " "<<myParams2;
                }
                 outF <<"){\n";//Aqui está implicito el inicio de una función
            }

            keyFunc=true; //Activo que estoy adentro de una función

        }else if(inS == "endFunc")
        {
            outF << "\n}" <<endl;

        }else if(inS == "roboTemp")
        {
            outF << "\ntemp = robot.GetTemp();\n\t "
                 << "Serial.print(\"Temperatura: \"); \n\tSerial.println(temp);\n";

        }else if(inS == "roboDistance")
        {
            outF << "\ndistance=robot.GetDistance();\n\t "
                 << "Serial.print(\"Distancia: \"); \n\tSerial.println(distance);\n";

        }else if(inS == "roboRightLight")
        {
            outF << "\nlightR=robot.LightSensorRight();\n\t "
                 << "Serial.print(\"Luz Derecha: \"); \n\tSerial.println(lightR);\n";

        }else if(inS == "roboLeftLight")
        {
            outF << "\nlightL=robot.LightSensorLeft();\n\t "
                 << "Serial.print(\"Luz Izquierda: \"); \n\tSerial.println(lightL);\n";

        }else if(inS == "roboRGB")
        {
            string r,g,b;
            inF >>r>>g>>b;
            outF << "\trobot.ColorRGB("<<r<<","<<g<<","<<b<<");"<<endl;

        }else if(inS == "roboOnLED")
        {
            string var;
            inF>>var;
            outF<<"\trobot.TurnOnLED("<<var<<");"<<endl;

        }else if(inS == "roboOffLED")
        {
            string var;
            inF>>var;
            outF<<"\trobot.TurnOffLED("<<var<<");"<<endl;

        }else if(inS == "roboBuzzer")
        {
            string time,note;
            inF>>time>>note;

            //outF<<"\trobot.SoundOn("<<time<<","<<convertToTone(note)<<");"<<endl;
            //EL robot tiene configurada la salida de la bocina en el puerto 2
            outF<<"\ttone( 2 ,"<<convertToTone(note)<<" , "<<time<<");"<<endl
                << "\tdelay("<<time<<"); noTone(2);\n";


        }else if(inS == "roboForward")
        {
            outF << "\trobot.MoveRoboTime(200,200,1000);\n";

        }else if(inS == "roboBack")
        {
            outF << "\trobot.MoveRoboTime(-200,-200,1000);\n";

        }else if(inS == "roboLeft")
        {
            outF << "\trobot.MoveRoboTime(200,0,500);\n";

        }else if(inS == "roboRight")
        {
            outF << "\trobot.MoveRoboTime(0,200,500);\n";

        }else if(inS == "startBlock")
        {
            if(!keyFunc || ifflag ||elseflag||whileflag||forflag)
                outF << "{" <<endl;

        }else if(inS == "endBlock")
        {
            if(!keyFunc)
                outF << "\n}" <<endl;
            keyFunc=false;
            ifflag=false;
            elseflag=false;
            whileflag=false;
            forflag=false;

        }else if(inS == "print")//PONER INICIO Y FIN DE PARAMETROS DEL IF
        {
            string myprint="";
            inF>>inS;//SOrdeo el parentesis

            inF>>inS;//Leo para saber que viene
            if(inS=="*"||inS=="/"||inS=="%"||inS=="+"||inS=="-"||inS=="<"||inS==">"
                    ||inS=="=="||inS=="!="||inS=="="||inS=="&&"||inS=="||"||inS=="(")
             {
                cuadruplos.push_back(inS);
                while(inF>>inS && inS != "endPrint")
                {
                    cuadruplos.push_back(inS);
                }
                    cuadruplos.pop_back();
                outF<<"\tSerial.println("<<generateExpression()<<");"<<endl;
             }
             else
             {
                myprint+="("+inS+" ";
                while(inF>>inS && inS != "endPrint")
                    myprint+=inS+" ";
                outF << "\tSerial.println"<<myprint<<";" <<endl;
             }


        }else if(inS == "while")//PONER INICIO Y FIN FIN DE PARAMETROS DEL WHILE
        {
            whileflag=true;
            inF>>inS;//Dummy de parentesis sordeado
            while(inF>>inS && inS != "endWhileParam")
                cuadruplos.push_back(inS);
            cuadruplos.pop_back();
            outF << "\twhile("<<generateExpression()<<")"<<endl;

        }else if(inS == "if")//PONER INICIO Y FIN FIN DE PARAMETROS DEL WHILE
        {
            ifflag=true;
            inF>>inS;//Dummy de parentesis sordeado
            while(inF>>inS && inS != "endIfParam")
                cuadruplos.push_back(inS);
            cuadruplos.pop_back();
            outF << "\tif("<<generateExpression()<<")"<<endl;

        }
        else if(inS == "else")//PONER INICIO Y FIN FIN DE PARAMETROS DEL WHILE
        {
            elseflag=true;
            outF << "\nelse"<<endl;

        }
        else  if(inS=="for")
        {
            forflag=true;
            string inFor="";
            inF>>inS;
            inFor+=inS;//primer parentesis
            inF>>inS;
            inFor+=inS;//primera expresion
             while(inF>>inS && inS!=";")
                cuadruplos.push_back(inS);
            inFor+=generateExpression()+"; ";
            while(inF>>inS && inS!=")")
                inFor+=inS;

                outF<<"for"<<inFor<<")";

        }else if(inS=="boolean"||inS=="string"||inS=="float")
        {
            string myvar="";

            if(inS=="string")
                outF<<"String"<<" ";
            else
                outF<<endl<<inS<<" ";

            while(inF>>inS && inS!= ";")
            {
                  if(inS=="string") myvar+="String  ";
                else
                    myvar+=inS;
            }

            outF<<myvar<<";\n";
        }else  if(inS=="call")
        {
            string inCall="";
            while(inF>>inS && inS!=";")
                inCall+=inS;
            outF<<inCall<<";"<<endl;
        }else
        {
            if(inS=="*"||inS=="/"||inS=="%"||inS=="+"||inS=="-"||inS=="<"||inS==">"||
                    inS=="=="||inS=="!="||inS=="="||inS=="&&"||inS=="||")
             {
                cuadruplos.push_back(inS);
                while(inF>>inS && inS != ";")
                {
                    cuadruplos.push_back(inS);
                    //cout<<inS<<endl;
                }
                //cout<<"YA SALÍ"<<endl;
                outF<<generateExpression()<<";"<<endl;
            }
            else
            {
                outF<<inS<<" ";
            }
        }

    }
    outF.close();
    inF.close();
    return 0;
}
