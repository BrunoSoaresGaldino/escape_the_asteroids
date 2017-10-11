
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define _MAX( a , b ) (  (a) > (b) ? (a) : (b) )
#define _MIN( a, b) ( (a) < (b) ? (a):  (b) )


volatile bool exit_program;

volatile int tick_counter;


void ExitProgram( )
{
    exit_program = true;
    
}
END_OF_FUNCTION(ExitProgram)

void IncrementTick( )
{
    
    tick_counter++;

}
END_OF_FUNCTION(IncrementTick)

typedef struct
{
    float x;
    float y;
    
    BITMAP* skin;
    
}Obj;

bool Colision( BITMAP *obj_a , BITMAP *obj_b, int x_a, int y_a,int x_b, int y_b ) 
{
    
    int i,j;
    
    if( !(  ( x_a > ( x_b + obj_b->w ) ) || ( y_a > ( y_b + obj_b->h  ) ) || ( x_b > ( x_a + obj_a->w  ) ) || ( y_b > ( y_a + obj_a->h  ) ) )  )
    {
 
        int top = _MAX( y_a , y_b );
        
        int bottom = _MIN( y_a + obj_a->h , y_b + obj_b->h );
        
        int left = _MAX( x_a, x_b );
        
        int right = _MIN( x_a + obj_a->w , x_b + obj_b->w );
        
        for( i = top ; i < bottom ; i++ )
        {
            for( j = left; j < right; j++ )
            {
                
                if( ( getpixel( obj_a, j - x_a, i - y_a ) != makecol( 255 , 0 , 255 ) ) && (getpixel( obj_b , j - x_b , i - y_b ) != makecol(255 , 0 , 255 ) ) );
                {
                    return true;
                }
            
            }
            
            
        }
        
        
        
    }
    
    return false;
    
}

int main(void)
{

    
    srand( (unsigned) time(NULL));
    
    BITMAP *buffer;
    BITMAP *level;
    BITMAP *spacecraft;
    BITMAP *black_hole;
    BITMAP *asteroid;
    BITMAP *heart;

    Obj    black_holes[50];
    Obj    asteroids[50];
    Obj    hearts[3];
    Obj    player = { 520, 400,spacecraft};

    double buffer_y = 0;
    double teta = 0.0;
    double level_speed = 5;
    
    int    ASTEROIDS = 5;
    int    BLACK_HOLES = 3;
    int    HEARTS = 2;
    
    int    level_counter = 0;
    int    lives = 3;
    
    int    i;
    
    
    
    allegro_init();
    install_timer();
    install_mouse();
    install_keyboard();
    
    exit_program = false;
    
    LOCK_FUNCTION(ExitProgram);
    LOCK_VARIABLE(exit_program);
    
    tick_counter = 0;
    
    LOCK_FUNCTION(IncrementTick);
    LOCK_VARIABLE(tick_counter);
    
    install_int_ex( IncrementTick , BPS_TO_TIMER(60) );
    
    set_close_button_callback(ExitProgram);
    
    set_color_depth(24);
    
    if ( set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0) ) 
    {
        
        fputs("Erro ao iniciar a Allegro",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    
    if( !buffer )
   {
        
        fputs("Erro ao criar buffer",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    level = load_bitmap("../image/level1.bmp",NULL);
    
    if( !level )
    {
        
        fputs("Erro ao carregar imagem de level",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    
    spacecraft = load_bitmap("../image/nave.bmp",NULL);
    
    if( !spacecraft )
    {
        
        fputs("Erroa ao carregar imagem da nave",stderr);
        exit(EXIT_FAILURE); 
        
        
    }
    
    black_hole = load_bitmap("../image/black_hole1.bmp",NULL);
    
    if( !black_hole )
    {
        
        fputs("Erro ao carregar imagem do buraco negro",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    asteroid = load_bitmap("../image/asteroid1.bmp",NULL );
    
    if( !asteroid )
    {
        fputs("Erro ao carregar imagem do asteroide",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    heart = load_bitmap("../image/heart.bmp",NULL);
    
    if( !heart )
    {
        fputs("Erro ao carregar imagem de coracao",stderr);
        exit(EXIT_FAILURE);
        
    }        
    
    
    while( !exit_program )
    {
        
        
        while( tick_counter > 0)
        {
        
        //USER INPUT
        
        if( key[ KEY_ESC ] )
        {    
            ExitProgram( );
        }
        
        if( key[ KEY_LEFT ] )
        {
            if( player.x > 1 )
            {
                if( player.x >= 10)
                {
                    player.x -= 10;
                }
                else 
                {
                    player.x -= 2;
                }
              
            }
        }
        
        if( key[ KEY_RIGHT ] )
        {
            
            if( player.x + 10 < SCREEN_W - spacecraft->w )
            {   
                if( player.x+10 < SCREEN_W - spacecraft->w )
                {
                    player.x += 10;
                }
                else 
                {
                    player.x +=2;
                }
            
            }
        }
        
        if( key[ KEY_UP ] )
        {
            if( player.y > 10 )
                player.y -= 10;
        }
        
        if( key[ KEY_DOWN ] )
        {
            
            if( player.y + 10 < SCREEN_H - spacecraft->h )
                player.y += 10;
           
        }
        
        if( key[ KEY_SPACE ] )
        {
            key[KEY_SPACE] = 0;
            
            while( !key[ KEY_SPACE ] )
            {
                
               
                
                
            }
            
            key[KEY_SPACE] = 0;
            
        }
        
        //ATUALIZACOES
        
        if( buffer_y > 0)
        {
            buffer_y -= level_speed;
        } 
        
        else 
        {
            
            buffer_y = level->h - SCREEN_H;
            
            level_counter++;
            
            ASTEROIDS++;
            
            BLACK_HOLES++;
            
            if(level_counter % 3 == 0 )
            {
                level_speed += 0.5;
            }
            
        }
        
        for( i = 0 ; i < BLACK_HOLES ; i++ )
        {
            if( black_holes[i].y > SCREEN_H )
            {
                black_holes[i].x = rand( ) % SCREEN_W;
                black_holes[i].y = rand() % SCREEN_H - SCREEN_H;
                black_holes[i].skin = black_hole;
            }
            else 
            {
                black_holes[i].y += level_speed;
            }
        }
      
        for( i = 0 ; i < ASTEROIDS ;i++ )
        {
            if( asteroids[i].y > SCREEN_H )
            {
                asteroids[i].x = rand( ) % SCREEN_W;
                asteroids[i].y = rand() % SCREEN_H - SCREEN_H;
                asteroids[i].skin = asteroid;
            }
            else
            {
                asteroids[i].y += 1.2*level_speed;
            }
        }
        
        for( i = 0 ; i < HEARTS ;i++ )
        {
            if( hearts[i].y > SCREEN_H )
            {
                hearts[i].x = ( rand( ) % SCREEN_W );
                hearts[i].y = ( rand() % SCREEN_H ) - 6*SCREEN_H;
                hearts[i].skin  = heart;
            }
            else
            {
                hearts[i].y += 1.5*level_speed;
            }
        }      

        for( i = 0; i < BLACK_HOLES; i++ )
        {
            if( Colision( spacecraft , black_hole , player.x , player.y , black_holes[i].x , black_holes[i].y ) )
            {   
                black_holes[i].y = SCREEN_H+1;
                lives--;
                break;
            }
            
        
        }
        
        for( i = 0; i < ASTEROIDS; i++ )
        {
            if( Colision( spacecraft , asteroid , player.x , player.y , asteroids[i].x , asteroids[i].y ) )
            {
                asteroids[i].y = SCREEN_H+1;
                lives--;
                break;
            }
       
        }
        
        for( i = 0; i < HEARTS ; i++ )
        {
            if( Colision( spacecraft , heart , player.x , player.y , hearts[i].x, hearts[i].y) )
            {
                hearts[i].y = SCREEN_H+1;
                lives++;
                break;
            }
        
        }        
        
        if( lives <= 0 )
        {
            ExitProgram();
        }
        
        teta += 0.2;
       
       //DESENHO
       
        blit(level, buffer , 0, (int)buffer_y , 0 , 0 ,SCREEN_W,SCREEN_H );
        
        for( i = 0 ; i < BLACK_HOLES ; i++ )
        {
            if( black_holes[i].y + black_hole->h > 0)
            {    
               
                rotate_sprite( buffer , black_hole , black_holes[i].x , black_holes[i].y , ftofix(-teta));

            }
        }
        
        for( i = 0 ; i < ASTEROIDS ; i++ )
        {
            if( asteroids[i].y + asteroid->h > 0 )
            {
             
                rotate_sprite( buffer , asteroid , asteroids[i].x , asteroids[i].y , ftofix(teta) );
 
            }
            
        }
        
        for( i = 0 ; i < HEARTS ; i++ )
        {
            if( hearts[i].y + heart->h > 0 )
            {

                draw_sprite( buffer , heart , hearts[i].x , hearts[i].y );
           
            }           
        }

        draw_sprite(buffer,spacecraft,player.x,player.y);
        
        textprintf_ex(buffer,font,2,2,makecol(255,255,255),-1,"level :%d",level_counter);
        textprintf_ex(buffer,font,2,12,makecol(255,255,255),-1,"lives :%d",lives);
        
        draw_sprite(screen,buffer,0,0);
        
        rest(0);
    
        tick_counter--;
        
        }
    
    }
    
}
END_OF_MAIN()