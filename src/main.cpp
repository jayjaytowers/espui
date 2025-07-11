#include <WiFi.h>
#include <ESPUI.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Callbacks para los diferentes controles
void buttonCallback (Control *sender, int type) {
  Serial.print ("Botón presionado, ");
  Serial.println (type);
}

void switchCallback (Control *sender, int type) {
  Serial.print ("Interruptor: ");
  Serial.println (sender -> value.c_str ());
}

void sliderCallback (Control *sender, int type) {
  Serial.print ("Deslizador: ");
  Serial.println (sender -> value.c_str());
}

void numberCallback (Control *sender, int type) {
  Serial.print ("Número: ");
  Serial.println (sender -> value.c_str());
}

void textCallback (Control *sender, int type) {
  Serial.print ("Texto ingresado: ");
  Serial.println (sender -> value.c_str());
}

void selectCallback (Control *sender, int type) {
  Serial.print ("Selector: ");
  Serial.println (sender -> value.c_str());
}

void padCallback (Control *sender, int type) {
  switch (type)
  {
  case -4:
    Serial.println ("Arriba");
    break;
  case 4:
    Serial.println ("Solto Arriba");
    break;
  case -3:
    Serial.println ("Derecha");
    break;
  case 3:
    Serial.println ("Solto Derecha");
    break;
  case -5:
    Serial.println ("Abajo");
    break;
  case 5:
    Serial.println ("Solto Abajo");
    break;
  case -2:
    Serial.println ("Izquierda");
    break;
  case 2:
    Serial.println ("Solto Izquierda");
    break;
  default:
    break;
  }
}

uint16_t graphId = ESPUI.graph ("Gráfico", ControlColor::Turquoise);

void setup() {
  Serial.begin (115200);
  WiFi.begin (ssid, password);

  Serial.print ("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }

  Serial.print (" WiFi conectado, IP: ");
  Serial.println (WiFi.localIP());

  ESPUI.begin ("Ejemplo completo de ESPUI");

  // Controles
  ESPUI.label ("Etiqueta", ControlColor::Alizarin, "Texto estático");

  ESPUI.button ("Botón", buttonCallback, ControlColor::Peterriver, "Dale click");

  ESPUI.switcher ("Interruptor", switchCallback, ControlColor::Sunflower, true);

  ESPUI.slider ("Deslizador", sliderCallback, ControlColor::Turquoise, 50, 0, 100);

  ESPUI.number ("Número", numberCallback, ControlColor::Carrot, 10);

  ESPUI.text ("Ingresa un texto", textCallback, ControlColor::Emerald, "Hola Manola");

  ESPUI.pad ("Mando", padCallback, ControlColor::Wetasphalt);
}

void loop() {
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > 5000) {
    // Agregar datos al gráfico
    ESPUI.addGraphPoint (graphId, random (0, 100), -1);
    lastUpdate = millis();
  }
}