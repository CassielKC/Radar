int trig=9; //declracion de pines
int echo=8; //declaracion de pines
int edo_ante_ban=0, ban=0, angulo=10, prom_angu=0, contobj=0, l, distancia=0, j, h, sum_dist=0, aux, prom_dist=0, sum_ang=0, i,vec_angulo[5]={0, 0, 0, 0, 0}, vec_cont[5]={0, 0, 0, 0, 0}, vec_dist[5]={0, 0, 0, 0, 0}, vec_cont_ante[5]={0, 0, 0, 0, 0}, vec_x[5]={0, 0, 0, 0, 0}, vec_y[5]={0, 0, 0, 0, 0}; 
float  duracion=0, radianes, x, y;
//solo libreria
#include <Servo.h>  //cualquier pin se puede ocuapra para poner servomotores

Servo servo_radar; //nombre del servomotor

void setup() 
{
  servo_radar.attach(6); //pin donde se usará el servo (digital)
  Serial.begin(9600);
  pinMode(trig, OUTPUT); //Trig envia info
  pinMode(echo, INPUT); //Echo recibe info
} 

void loop() 
{
  
  //int vec_angulo[5]={0, 0, 0, 0, 0}, vec_cont[5]={0, 0, 0, 0, 0}, vec_dist[5]={0, 0, 0, 0, 0}, ban=0;
  int cont_valor=0, k, ban2=0, auxmax=0, auxmin=0, edo_ante_ban = 0, ban=0, distancia=0, sum_dist=0, sum_ang=0, x=0, y=0;
  servo_radar.write(180);
  delay(1000);
  //giro del servo
  for(i=180; i>=20; i--)
  {
    //manda onda para sacar distancia
    digitalWrite(trig, LOW);//tener disparo limpio de sensor ultra
    delayMicroseconds(4);
    digitalWrite(trig, HIGH);
    delayMicroseconds (10);
    digitalWrite(trig, LOW);
    duracion = pulseIn(echo, HIGH);
    
    distancia = duracion*0.034/2; //calculo de distancia
    
    //distancia=duracion/29.2;
    if(distancia<56 && angulo<169) //checa si la distancia a la que esta el objeto es menor a 55
    {
      //si si
      edo_ante_ban = ban;
      ban = 1;
      sum_dist=sum_dist+distancia;
      sum_ang=sum_ang+angulo;
      contobj++;
    }
    else //si no quiere decir que la distancia fue mayor a 55cm
    {
      ban=0;
      if(edo_ante_ban==1)//si el estado anterior de la bandera es 1 quiere decir que acaba de ver un objeto
      {
        //checar si esto no afecta
        edo_ante_ban=ban;
        /*Serial.println(" suma distancia: ");
        Serial.println(sum_dist);
        Serial.println(' ');
        Serial.println("suma angulo: ");
        Serial.println(sum_ang);*/
        prom_dist=sum_dist/contobj;
        prom_angu=sum_ang/contobj;
        radianes=prom_angu*3.1416/180;
        x=prom_dist*cos(radianes);
        y=prom_dist*sin(radianes);
        sum_dist=0;
        sum_ang=0;
        contobj=0;
        //temporal
        /*Serial.println("------------------");
        Serial.println("distancia: ");
        Serial.println(prom_dist);
        Serial.println(' ');
        Serial.println("angulo: ");
        Serial.println(prom_angu);
        Serial.println("------------------");
        Serial.print("coordenada en x:");
        Serial.println(x);
        Serial.print("coordenada en y:");
        Serial.println(y);
        Serial.println("------------------");*/
     //checar si el valor existe
        for(l=0; l<5; l++) //&&ban2!=1
        {
          auxmax=vec_angulo[l]+5;
          auxmin=vec_angulo[l]-5;
          if(auxmin<=prom_angu && auxmax>=prom_angu && prom_angu!=0)
          {
            //ban2=1;
            //si el valor si se parece entonces aumenta el contador en esa posicion
            vec_cont[l]=vec_cont[l]+1;
            prom_angu=0;
            prom_dist=0;
            x=0;
            y=0;
            //checa si el contador ya es mayor a 3 
            /*if(vec_cont[l]>3) // si si es mayor a 3, entonces lo borra
            {
              vec_angulo[l]=0;
              vec_dist[l]=0;
              vec_cont[l]=0;
            }*/
          }
          else //si no existe el valor entonces
          {
            //busca un lugar vacio
            if(vec_angulo[l]==0 && prom_angu!=0) //si el lugar esta vacio, entonces...
            {
              //ban2=1;
              aux=l;
              vec_angulo[aux]=prom_angu;
              vec_dist[aux]= prom_dist;
              vec_x[aux]=x;
              vec_y[aux]=y;
              vec_cont[aux]=vec_cont[aux]+1;
              prom_angu=0;
              prom_dist=0;
              l=5;
            }
            //checar, si el valor se parece
          }
          cont_valor++; //si el valor llega a 5 quiere decir que el vector esta lleno y se quiere incertar un nuevo objeto// y si no se parece regresa al for
        }//for
      }//if donde checa si el valor de la bandera anterior era 1
      if(cont_valor==5 && prom_angu!=0) //si el contador llego a 5 quiere decir que el vector está lleno
      {
        for(h=0; h<5; h++)
        {
          if(vec_cont[h]>3)
          {
            vec_angulo[h]=prom_angu;
            vec_dist[h]= prom_dist;
            vec_x[h]=x;
            vec_y[h]=y;
            vec_cont[h]=1;
            h=6;
          }//if que checa si algun contador es mayor a 3
        }//for pars checar el vector contador y ver si alguno es mayor a 3
      }
      
      /*else
      {
        edo_ante_ban=ban;
        ban=0;
        sum_dist=0;
        sum_ang=0;
        prom_dist=0;
        prom_angu=0;
        contobj=0;
      }*/
      
    }//else la ditancia fue mayor a 60
    servo_radar.write(i);
    angulo++;
    delay(150);
  }//for giro del servo
  for(k=0; k<5; k++)
  {
    if(vec_cont_ante[k]==vec_cont[k])
    {
      vec_angulo[k]=0;
      vec_dist[k]=0;
      vec_x[k]=x;
      vec_y[k]=y;
      vec_cont[k]=0;
      vec_cont_ante[k]=0;
    }
    else
    {
      vec_cont_ante[k]=vec_cont[k];
    }
  }//for de comparador de contadores
  /*Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j]!=0 && vec_cont[j]<4)
    {
      //Serial.println(' ');
      Serial.print("angulo numero: ");
      Serial.print(j+1);
      Serial.print(' ');
      Serial.println(vec_angulo[j]);
    }
  }
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j]!=0 && vec_cont[j]<4)
    {
      Serial.print("distancia numeor: ");
      Serial.print(j+1);
      Serial.print(' ');
      Serial.println(vec_dist[j]);
    }
  }*/
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j]!=0 && vec_cont[j]<4)
    {
       Serial.println("coordenadas del objeto ");
       Serial.println(j+1);
       Serial.print("********x :");
       Serial.print(vec_x[j]);
       Serial.println("********");
       Serial.print("********y :");
       Serial.print(vec_y[j]);
       Serial.println("********");
    }
  }
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    Serial.print("contador de la casilla: ");
    Serial.print(j+1);
    Serial.print(' ');
    Serial.println(vec_cont[j]);
  }
  Serial.println("___________________________");
  angulo=170;
  delay(2000);

  for(i=20; i<180; i++)
  {
   //manda onda para sacar distancia
    digitalWrite(trig, LOW);//tener disparo limpio de sensor ultra
    delayMicroseconds(4);
    digitalWrite(trig, HIGH);
    delayMicroseconds (10);
    digitalWrite(trig, LOW);
    duracion = pulseIn(echo, HIGH);
    
    distancia = duracion*0.034/2; //calculo de distancia
    
    //distancia=duracion/29.2;
    if(distancia<56 && angulo>11) //checa si la distancia a la que esta el objeto es menor a 55
    {
      //si si
      edo_ante_ban = ban;
      ban = 1;
      sum_dist=sum_dist+distancia;
      sum_ang=sum_ang+angulo;
      contobj++;
    }
    else //si no quiere decir que la distancia fue mayor a 55cm
    {
      if(edo_ante_ban==1)//si el estado anterior de la bandera es 1 quiere decir que acaba de ver un objeto
      {
        ban=0;//checar sie sto no afecta
        edo_ante_ban=ban;
        /*Serial.println(" suma distancia: ");
        Serial.println(sum_dist);
        Serial.println(' ');
        Serial.println("suma angulo: ");
        Serial.println(sum_ang);*/
        prom_dist=sum_dist/contobj;
        prom_angu=sum_ang/contobj;
        radianes=prom_angu*3.1416/180;
        x=prom_dist*cos(radianes);
        y=prom_dist*sin(radianes);
        sum_dist=0;
        sum_ang=0;
        contobj=0;
        //temporal
        /*Serial.println("------------------");
        Serial.println("distancia: ");
        Serial.println(prom_dist);
        Serial.println(' ');
        Serial.println("angulo: ");
        Serial.println(prom_angu);
        Serial.println("------------------");
        Serial.print("coordenada en x:");
        Serial.println(x);
        Serial.print("coordenada en y:");
        Serial.println(y);
        Serial.println("------------------");*/
     //checar si el valor existe
        for(l=0; l<5; l++) //&&ban2!=1
        {
          auxmax=vec_angulo[l]+5;
          auxmin=vec_angulo[l]-5;
          if(auxmin<=prom_angu && auxmax>=prom_angu && prom_angu!=0)
          {
            //ban2=1;
            //si el valor si se parece entonces aumenta el contador en esa posicion
            vec_cont[l]=vec_cont[l]+1;
            prom_angu=0;
            prom_dist=0;
            //checa si el contador ya es mayor a 3 
            /*if(vec_cont[l]>3) // si si es mayor a 3, entonces lo borra
            {
              vec_angulo[l]=0;
              vec_dist[l]=0;
              vec_cont[l]=0;
            }*/
          }
          else //si no existe el valor entonces
          {
            //busca un lugar vacio
            if(vec_angulo[l]==0 && prom_angu!=0) //si el lugar esta vacio, entonces...
            {
              //ban2=1;
              aux=l;
              vec_angulo[aux]=prom_angu;
              vec_dist[aux]= prom_dist;
              vec_x[aux]=x;
              vec_y[aux]=y;
              vec_cont[aux]=vec_cont[aux]+1;
              prom_angu=0;
              prom_dist=0;
              l=5;
            }
            //checar, si el valor se parece
          }
          cont_valor++; //si el valor llega a 5 quiere decir que el vector esta lleno y se quiere incertar un nuevo objeto// y si no se parece regresa al for
        }//for
      }//if donde checa si el valor de la bandera anterior era 1
      if(cont_valor==5 && prom_angu!=0) //si el contador llego a 5 quiere decir que el vector está lleno
      {
        for(h=0; h<5; h++)
        {
          if(vec_cont[h]>3)
          {
            vec_angulo[h]=prom_angu;
            vec_dist[h]= prom_dist;
            vec_cont[h]=1;
            h=6;
          }//if que checa si algun contador es mayor a 3
        }//for pars checar el vector contador y ver si alguno es mayor a 3
      }
      
      /*else
      {
        edo_ante_ban=ban;
        ban=0;
        sum_dist=0;
        sum_ang=0;
        prom_dist=0;
        prom_angu=0;
        contobj=0;
      }*/
      
    }//else la ditancia fue mayor a 60
    servo_radar.write(i);
    angulo--;
    delay(150);
  }
  for(k=0; k<5; k++)
  {
    if(vec_cont_ante[k]==vec_cont[k])
    {
      vec_angulo[k]=0;
      vec_dist[k]=0;
      vec_cont[k]=0;
      vec_cont_ante[k]=0;
    }
    else
    {
      vec_cont_ante[k]=vec_cont[k];
    }
  }//for de comparador de contadores
  /*Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j]!=0 && vec_cont[j]<4)
    {
      //Serial.println(' ');
      Serial.print("angulo numero: ");
      Serial.print(j+1);
      Serial.print(' ');
      Serial.println(vec_angulo[j]);
    }
    
  }
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j]!=0 && vec_cont[j]<4)
    {
      Serial.print("distancia numeor: ");
      Serial.print(j+1);
      Serial.print(' ');
      Serial.println(vec_dist[j]);
    }
    
  }*/
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    if(vec_angulo[j] && vec_cont[j]<4)
    {
       Serial.println("coordenadas del objeto ");
       Serial.println(j+1);
       Serial.print("********x :");
       Serial.print(vec_x[j]);
       Serial.println("********");
       Serial.print("********y :");
       Serial.print(vec_y[j]);
       Serial.println("********");
    }
  }
  Serial.println("---------------------------");
  for(j=0; j<5; j++)
  {
    Serial.print("contador de la casilla: ");
    Serial.print(j+1);
    Serial.print(' ');
    Serial.println(vec_cont[j]);
  }
  Serial.println("___________________________");
  angulo=10;
  delay(3000);
}
