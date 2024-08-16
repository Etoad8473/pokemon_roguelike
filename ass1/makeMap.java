import java.util.Arrays;
import java.util.Random;


class makeMap
{

    int height = 20;
    int width = 60;

    char[][] map = new char[height][width];
    Random r = new Random();

    public static void main(String args[])
    {
        makeMap m = new makeMap();
        m.generateMap();
        m.printMap();
    }
    
    public makeMap(){}

    public void generateMap()
    {
        int hM = height -1;
        int wM = width -1;
        
        //grass
        fillRect('.', 0, 0, hM, wM);
        //tall grass
        fillRect(':',getRand(0, hM),getRand(0, wM),getRand(0, hM),getRand(0, wM));
        sprinkle(':', getRand(0, 20));
        //water
        fillRect('~',getRand(0, hM),getRand(0, wM),getRand(0, hM),getRand(0, wM));
        //mountains
        fillRect('%',getRand(0, hM),getRand(0, wM),getRand(0, hM),getRand(0, wM));
        sprinkle('%', getRand(0, 5));
        //trees
        fillRect('^',6,3,5,1);
        sprinkle('^', getRand(0, 10));
        //make border lines: %
        drawBorder('%');
        //roads: #
        drawRoads();

    }

    public void fillRect(char terrain, int y1IN, int x1IN, int y2IN, int x2IN)
    {
        int yMin, yMax, xMin, xMax;
        yMax = map.length - 1;
        xMax = map[0].length - 1;
        yMin = 0;
        xMin = 0;

        int y1, x1, y2, x2;

        //FORMAT & BOUND INPUT
        if(x1IN > x2IN){
            x1 = x2IN; x2 = x1IN;
        }
        else{
            x1 = x1IN; x2 = x2IN;
        }
        if(y1IN > y2IN){
            y1 = y2IN; y2 = y1IN;
        }
        else{
            y1 = y1IN; y2 = y2IN;
        }

        if(x1 < xMin){
            x1 = xMin;
        }
        if(y1 < yMin){
            y1 = yMin;
        }
        if(x2 > xMax){
            x2 = xMax;
        }
        if(y2 > yMax){
            y2 = yMax;
        }
        
        //starting at y1 for #of rows
            //starting at x1 for #of columns
                //map[i][j] = char
        for(int i = y1; i <= y2; i++)
        {
            for(int j = x1; j <= x2; j++)
            {
                map[i][j] = terrain;
            }
        }

    }

    //draw border
    public void drawBorder(char terrain)
    {
        int hM = height - 1;
        int wM = width - 1;
        fillRect(terrain, 0, 0, 0, wM);
        fillRect(terrain, 0, 0, hM, 0);
        fillRect(terrain, hM, 0, hM, wM);
        fillRect(terrain, 0, wM, hM, wM);
    }

    
    public void sprinkle(char terrain, int number)
    {
        for(int i = 0; i < number; i++)
        {
            map[getRand(0, height-1)][getRand(0, width-1)] = terrain;
        }
    }

    public void drawRoads()
    {

        //choose two y's, x to split
        int y1 = getRand(3, height -4);
        int y2 = getRand(3, height -4);
        int x1 = getRand(3, width -4);
        
        //Draw pokemarts & center
        martsOnRoad(y1, y2, x1);

        fillRect('#', y1, 0, y1, x1);
        fillRect('#', y2, x1, y2, width -1);
        fillRect('#',y1,x1,y2,x1);

        y1 = getRand(2, height -3);
        x1 = getRand(2, width -3);
        int x2 = getRand(2, width -3);

        fillRect('#',0,x1,y1,x1);
        fillRect('#',y1,x2,height-1,x2);
        fillRect('#',y1,x1,y1,x2);

    }

    public void martsOnRoad(int y1, int y2, int x1)
    {
        int pX = getRand(2, x1-2);
        y1++;

        fillRect('p', y1, pX, y1+1, pX-1);

        int cX = getRand(x1, width -3- x1);
        y2--;
        fillRect('c',y2,cX,y2-1,cX+1);

    }

    // public void swap(int a, int b)
    // {
    //     System.out.println("before swap: " + a + " " + b);
    //     int temp = a;
    //     a = b;
    //     b = temp;
    //     System.out.println("before after: " + a + " " + b);

    // }

    public int getRand(int lowest, int size)
    {
        int out = lowest + r.nextInt(size);
        System.out.println(out);
        return out;
    }

    public void printMap()
    {
        for(int i = 0; i < map.length; i++)
        {
            for(int j = 0; j < map[0].length; j++)
            {
                System.out.printf(" %s", map[i][j]);
            }
            System.out.print("\n");
        }
    }
}