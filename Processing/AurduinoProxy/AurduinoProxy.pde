import processing.serial.*;
import java.io.*;
import java.util.Map;
import java.util.Arrays;

Serial serial;

interface IServer {

  void handleMessage(int selector, char[] buffer, int bufferSize);

}

HashMap<Integer, IServer> proxies = new HashMap<Integer, IServer>();

class WindowServer implements IServer {

    static final int DrawPoint = 1;
    static final int DrawChar = 2;
  
    void handleMessage(int selector, char[] buffer, int bufferSize)
    {
        if (selector == DrawPoint) {
            int x = buffer[0] << 0 | buffer[1] << 8;
            int y = buffer[2] << 0 | buffer[3] << 8;
    
            fill(random(128) + 128, random(128) + 128, random(128) + 128, 64);
            noStroke();
            ellipse(x, y, random(100), random(100));
            redraw();
        }
        
        if (selector == DrawChar) {
            String string = new String(buffer, 0, bufferSize);
            
            fill(255);
            rect(10, 10, textWidth(string) + 1, textAscent() + textDescent());
            
            fill(0x00);
            text(string, 10, 10 + textAscent());
            redraw();
        }
    }
  
}

char[] buffer = new char[100];
int state = -1, bufferIndex = 0;
int port, selector, messageSize;

void setup()
{
    size(1024, 768);
    noLoop();
    
    background(255);
                
    serial = new Serial(this, "/dev/cu.usbmodem1451", 9600);
    
    proxies.put(10, new WindowServer());
}

void keyPressed()
{
    if (keyCode == ENTER) {
        serial.write("\r\n");
    }
    else serial.write(key);
}

void draw()
{
}

void serialEvent(Serial serial)
{
    if (state == -1) {
        if (serial.read() == 'A') {
            state = 0;
        }
    }
    else if (state == 0) {
        port = serial.read();

        state = 1;
    }
    else if (state == 1) {
        selector = serial.read();
        
        state = 2;
    }
    else if (state == 2) {
        messageSize = serial.read();

        if (messageSize < 100) {
            state = 3;
        }
    }
    else if (state == 3) {
        buffer[bufferIndex] = serial.readChar();
        bufferIndex += 1;

        if (bufferIndex == messageSize) {

            IServer server = proxies.get(port);
            if (server != null) {
                server.handleMessage(selector, buffer, messageSize);
            }
                        
            bufferIndex = 0;
            state = 0;
        }
    }
}

