#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
//swap two integers
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp; 
}
bool BFS(int n,int maze[n][n],int step,int x, int y,int dest_x,int dest_y){ 
    //out of range or it is block
    if (step<2||step>n*n||x<0||x>=n||y<0||y>=n||maze[x][y]==1) 
        return false;
    //if the step number now is the lower than before, update it
    if (maze[x][y] == 0 || maze[x][y] > step) 
        maze[x][y] = step;
    else return false;
    //arrived at the destination return true
    if (x == dest_x && y == dest_y){
        return true;
    }
    bool is_path = false;
    //check four direction have path or not
    if (x+1<n && maze[x+1][y] != 1 && (maze[x+1][y] == 0 || 
        maze[x+1][y] > step+1) && BFS(n,maze,(step+1),x+1,y,dest_x,dest_y)) 
        is_path = true;
    if (x-1>=0 && maze[x-1][y] != 1 && (maze[x-1][y] == 0 || 
        maze[x-1][y] > step+1) && BFS(n,maze,(step+1),x-1,y,dest_x,dest_y)) 
        is_path = true;
    if (y+1<n && maze[x][y+1] != 1 && (maze[x][y+1] == 0 || 
        maze[x][y+1] > step+1) && BFS(n,maze,(step+1),x,y+1,dest_x,dest_y)) 
        is_path = true;
    if (y-1>=0 && maze[x][y-1] != 1 && (maze[x][y-1] == 0 || 
        maze[x][y-1] > step+1) && BFS(n,maze,(step+1),x,y-1,dest_x,dest_y)) 
        is_path = true;

    return is_path;
}
//clear n elements in action array 
void clear_action(int *path, int n){
    for (int *p = path; p < path+n;p++)
        *p = -1;
}
//check each step num of which directions is 1 less than current position's number
int find_previous_action(int n, int maze[n][n],int pos[2]){
    //up:0
    if (pos[0]+1 <= n && maze[pos[0]+1][pos[1]] == maze[pos[0]][pos[1]]-1) {
        pos[0]+=1;
        return 0;
    } 
    //right:1
    if (pos[1]-1 >= 0 && maze[pos[0]][pos[1]-1] == maze[pos[0]][pos[1]]-1){
        pos[1]-=1;
        return 1; 
    }      
    //down:2
    if (pos[0]-1 >= 0 && maze[pos[0]-1][pos[1]] == maze[pos[0]][pos[1]]-1){
        pos[0]-=1;
        return 2;
    } 
    //left:3
    if (pos[1]+1 <= n && maze[pos[0]][pos[1]+1] == maze[pos[0]][pos[1]]-1){
        pos[1]+=1;
        return 3; 
    }
    //no move:-1
    return -1;
}
//set inline to reduce time
inline void load_action(int n, int maze[n][n], int action[n*n],const int pos_ball[2],const int pos_dest[2]);
//load action from the map of the ball
void load_action(int n, int maze[n][n], int action[n*n],const int pos_ball[2],const int pos_dest[2]){
    //clear array
    for (int iter = 0; iter < n*n; iter++) action[iter] = -1;
    //it is at the destination now
    if (pos_ball[0] == pos_dest[0] && pos_ball[1] == pos_dest[1]) return;
    //the step number at the destination on the map is real step_num - 3
    int step_num = maze[pos_dest[0]][pos_dest[1]] - 3;
    //copy the destination position to become the first position
    //and look back to find the path
    int current_pos[2] = {pos_dest[0],pos_dest[1]};
    for (int step = step_num; step >= 0 && step < n*n; step--){
        //0: up 1: right 2: down 3: left 
        action[step] = find_previous_action(n,maze,current_pos);
    }
}
//copy n elements from res to des
void copy_map(int *res, int *des, int n){
    int *p = res, *s = des;
    for (; p < res+n && s < des+n; p++,s++)
        *s = *p;
}
//copy the original maze to the map for clearing the step
void clear_map(int *maze,int *map,int n){
    copy_map(maze,map,n);
}
//check the positions of ball and dest are the same or not
int is_arrived(int pos[2][2]){
    int is_arrived = ((pos[0][0] == pos[1][0]) && (pos[0][1] == pos[1][1]));
    return is_arrived;
}
//set inline to reduce time
inline int move_ball(int n,int pos[2], int direction, int maze[n][n]);
//move the position by direction
int move_ball(int n,int pos[2], int direction, int maze[n][n]){
    switch (direction){
    //up
    case 0: if((pos[0]-1)>=0)
                if(maze[pos[0]-1][pos[1]]!=1){
                    pos[0]-=1;
                    return 0;
                } break;
    //right
    case 1: if((pos[1]+1)<n)
                if(maze[pos[0]][pos[1]+1]!=1){
                    pos[1]+=1;
                    return 1;
                } break;
    //down
    case 2: if((pos[0]+1)<n)
                if(maze[pos[0]+1][pos[1]]!=1){
                    pos[0]+=1;
                    return 2;
                } break;
    //left
    case 3:if((pos[1]-1)>=0)
                if(maze[pos[0]][pos[1]-1]!=1){
                    pos[1]-=1;
                    return 3;
                } break;    
    default:
        break;
    }
    return -1;
}
//use the action to check whether ball can arrive dest or not
bool try_to_move(int n,int pos[2][2],int action[n*n],int maze[2][2]){
    int ball[2] = {pos[0][0],pos[0][1]};
    for (int iter = 0; iter < n*n&&action[iter]!=-1; iter++){
        move_ball(n,ball,action[iter],maze);
    } 
    return (ball[0]==pos[1][0] && ball[1]==pos[1][1]);
}
//find the first same direction as action[major][0] in action[minor]
//if there exist,return the index; if not, return -1
int find_same_direction(int n,int action[2][n*n],int major,int minor){
    for(int index = 0; index < n*n && action[minor][index] != -1 && action[major][0]; index++){
        if (action[major][0] == action[minor][index])
            return index;
    }
    return -1;
}
//left shift one index for the action array
void left_shift_action_array(int n, int action[n]){
    for (int index = 1;index < n; index++){
        action[index-1] = action[index];
    }
    action[n-1] = -1;
}
//check whether the minor's action would make major_ball move or not
//if major move, return true. Otherwise, return false
bool check_major_move(int n,int maze[n][n], int major_pos[2], int *minor_action ,int target_index){
    bool major_is_move = false;
    int temp_pos[2] = {major_pos[0], major_pos[1]};
    
    for (int index = 0; index < target_index && minor_action[index]!=-1; index++){
        if (move_ball(n,temp_pos,minor_action[index],maze) != -1){
            major_is_move = true;
            break;
        }
    }
    return major_is_move;
}
//the major function to find the path of two balls
void fastest_path(int n, int path[2*n*n], int maze[n][n],int map[2][n][n],const int pos_ball[2][2],const int pos_dest[2][2]){
    //initialize
    int major_ball = 0, minor_ball = 1, major_dest = 0, minor_dest = 1;
    int n_2 = n*n;
    int total_step[2][2] = {0};
    int action[2][n_2];
    clear_action(action[0],n_2);
    clear_action(action[1],n_2);
    //caculate the steps used by each pair and decided the major pair and minor pair
    for (int ball = 0; ball < 2; ball++){
        for (int dest = 0; dest < 2; dest++){
            clear_map(*maze,*map[ball],n_2);
            //the 2 is the first step number for the map
            if(BFS(n,map[ball],2,pos_ball[ball][0],pos_ball[ball][1],pos_dest[dest][0],pos_dest[dest][1])){
            }else {
                return;
                //printf("not found\n");
            }
            //process action
            clear_action(action[ball],n_2);
            load_action(n,map[ball],action[ball],pos_ball[ball],pos_dest[dest]);
            //minus 2 to get the real total steps
            total_step[ball][dest] = map[ball][pos_dest[dest][0]][pos_dest[dest][1]] - 2;
        }
    }
    //choose the least total step numbers between four pairs
    if (total_step[0][0]+total_step[1][1] < total_step[0][1]+total_step[1][0]){
        if (total_step[0][0] < total_step[1][1]){
            major_ball = 0; major_dest = 0;     
        }else {
            major_ball = 1; major_dest = 1;
        }  
    } else{
        if (total_step[0][1] < total_step[1][0]){
            major_ball = 0; major_dest = 1;     
        }else {
            major_ball = 1; major_dest = 0;
        }          
    }
    //set the other ball and destination to be minor pair
    minor_ball = (major_ball+1)%2;
    minor_dest = (major_dest+1)%2;
    //copy the corresponding position 
    //0:ball (row,col), 1:dest (row,col) 
    int major_pos[2][2] = {{pos_ball[major_ball][0], pos_ball[major_ball][1]}, 
                            {pos_dest[major_dest][0],pos_dest[major_dest][1]}}; 
    int minor_pos[2][2] = {{pos_ball[minor_ball][0], pos_ball[minor_ball][1]}, 
                            {pos_dest[minor_dest][0],pos_dest[minor_dest][1]}};
    //initialize the map and action array for the major and minor
    int step_num = 0;
    clear_action(action[major_ball],n_2);
    clear_action(action[minor_ball],n_2);
    clear_map(*maze,*map[major_ball],n_2);
    clear_map(*maze,*map[minor_ball],n_2);
    BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
    load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);
    BFS(n,map[major_ball],2,major_pos[0][0],major_pos[0][1],major_pos[1][0],major_pos[1][1]);
    load_action(n,map[major_ball],action[major_ball],major_pos[0], major_pos[1]);
    while(1){
        //out of range
        if (step_num>2*n_2) return;
        //store the pairs which switch the balls of major and minor 
        int mix[2][2][2] = {{{major_pos[0][0],major_pos[0][1]},{minor_pos[1][0],minor_pos[1][1]}},
                            {{minor_pos[0][0],minor_pos[0][1]},{major_pos[1][0],major_pos[1][1]}}};
        //if the action array is wrong for the pair,update it
        if (!try_to_move(n,major_pos,action[major_ball],maze)){
            clear_action(action[major_ball],n_2);
            clear_map(*maze,*map[major_ball],n_2);
            BFS(n,map[major_ball],2,major_pos[0][0],major_pos[0][1],major_pos[1][0],major_pos[1][1]);
            load_action(n,map[major_ball],action[major_ball],major_pos[0], major_pos[1]);            
        }
        if (!try_to_move(n,minor_pos,action[minor_ball],maze)){
            clear_action(action[minor_ball],n_2);
            clear_map(*maze,*map[minor_ball],n_2);
            BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
            load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);         
        }
        //check whether two balls arrived the another ball's destination
        //if it happen, switch major and minor ball, and update all the data
        if (is_arrived(mix[0])||is_arrived(mix[1])){
            swap(&major_pos[0][0],&minor_pos[0][0]);
            swap(&major_pos[0][1],&minor_pos[0][1]);
            clear_action(action[major_ball],n_2);
            clear_action(action[minor_ball],n_2);
            clear_map(*maze,*map[major_ball],n_2);
            clear_map(*maze,*map[minor_ball],n_2);
            BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
            load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);
            BFS(n,map[major_ball],2,major_pos[0][0],major_pos[0][1],major_pos[1][0],major_pos[1][1]);
            load_action(n,map[major_ball],action[major_ball],major_pos[0], major_pos[1]);
        }
        //check what the condition is now
        if (is_arrived(major_pos)){
            if (is_arrived(minor_pos)){
                for (int i = 0; i < n; i++){
                    for (int j = 0; j< n;j++){
                        printf("%2d ", map[major_ball][i][j]);
                    }
                    printf("\n");
                }
                //both arrived
                return;
            }else{
                //major arrived,minor not yet
                clear_map(*maze,*map[minor_ball],n_2);
                BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
                load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);
                //move the minor to destination
                for (int iter = 0; iter < n_2 && action[minor_ball][0]!=-1; iter++){
                    move_ball(n,minor_pos[0],action[minor_ball][0],maze);
                    assert(step_num<2*n_2);
                    path[step_num++] = action[minor_ball][0];
                    left_shift_action_array(n_2,action[minor_ball]);
                }
            }
        } else{
            if (is_arrived(minor_pos)){
                //major not yet,minor arrived
                clear_map(*maze,*map[major_ball],n_2);
                BFS(n,map[major_ball],2,major_pos[0][0],major_pos[0][1],major_pos[1][0],major_pos[1][1]);
                load_action(n,map[major_ball],action[major_ball],major_pos[0], major_pos[1]);
                //move the major to destination
                for (int iter = 0; iter < n_2&&action[major_ball][0]!=-1; iter++){
                    move_ball(n,major_pos[0],action[major_ball][0],maze);
                    assert(step_num<2*n_2);
                    path[step_num++] = action[major_ball][0];
                    left_shift_action_array(n_2,action[major_ball]);
                }                
            }else{
                //both not yet
                //store the index of the next same direction of two ball
                int target_index = find_same_direction(n,action,major_ball,minor_ball);
                if (target_index <= -1 || target_index >= n_2){
                    //out of range or there is not same direction
                    //move one step based on major ball
                    move_ball(n,major_pos[0],action[major_ball][0],maze);
                    move_ball(n,minor_pos[0],action[major_ball][0],maze);
                    assert(step_num<2*n_2);
                    path[step_num++] = action[major_ball][0];
                    left_shift_action_array(n_2,action[major_ball]);
                    //update the path of minor pair
                    clear_map(*maze,*map[minor_ball],n_2);
                    clear_action(action[minor_ball],n_2);
                    BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
                    load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);
                    
                }else if(target_index == 0){
                    //the next step of both of them are the same
                    move_ball(n,major_pos[0],action[major_ball][0],maze);
                    move_ball(n,minor_pos[0],action[minor_ball][0],maze);
                    assert(step_num<2*n_2);
                    path[step_num++] = action[major_ball][0];  
                    left_shift_action_array(n_2,action[major_ball]);
                    left_shift_action_array(n_2,action[minor_ball]);      
                }else{
                    //check whether the minor's action before target index
                    // would affect major or not
                    if (check_major_move(n,maze,major_pos[0],action[minor_ball],target_index)){
                        //it would affect major, so 
                        //move one step based on major ball
                        move_ball(n,major_pos[0],action[major_ball][0],maze);
                        move_ball(n,minor_pos[0],action[major_ball][0],maze);
                        assert(step_num<2*n_2);
                        path[step_num++] = action[major_ball][0];
                        left_shift_action_array(n_2,action[major_ball]);
                        //update minor ball's action and map
                        clear_map(*maze,*map[minor_ball],n_2);
                        clear_action(action[minor_ball],n_2);
                        BFS(n,map[minor_ball],2,minor_pos[0][0],minor_pos[0][1],minor_pos[1][0],minor_pos[1][1]);
                        load_action(n,map[minor_ball],action[minor_ball],minor_pos[0], minor_pos[1]);
                        
                    } else{
                        //major ball would not move by the action[minor_ball][0~targe_index-1]
                        for (int index = 0; index < target_index && action[minor_ball][0] != -1;index++){
                            //move based on minor'action
                            //major is suppose to be stay right there
                            move_ball(n,major_pos[0],action[minor_ball][0],maze);
                            move_ball(n,minor_pos[0],action[minor_ball][0],maze);
                            assert(step_num<2*n_2);
                            path[step_num++] = action[minor_ball][0]; 
                            left_shift_action_array(n_2,action[minor_ball]);
                        }
                    }
                }
            }
        }
    }
}
int main(void){
    //initialize
    int n = 0;
    scanf("%d", &n);
    int pos_ball[2][2]; 
    int pos_dest[2][2];
    int path[2*n*n];
    int maze[n][n];
    int n_2 = n*n;
    //scan maze
    for(int *p = *maze; p<*maze+n_2;p++){
        scanf("%d", p);
    }
    //scan position
    scanf("%d%d%d%d",&pos_ball[0][1],&pos_ball[0][0],&pos_ball[1][1],&pos_ball[1][0]);
    scanf("%d%d%d%d",&pos_dest[0][1],&pos_dest[0][0],&pos_dest[1][1],&pos_dest[1][0]);
    //change x,y into row,column
    for(int i = 0; i < 2; i++){
        pos_ball[i][0] = n-1-pos_ball[i][0];
        pos_dest[i][0] = n-1-pos_dest[i][0];
    }
    int map[2][n][n];
    //copy maps for two balls
    copy_map(*maze,*map[0],n_2);
    copy_map(*maze,*map[1],n_2);
    //clear the path array
    clear_action(path,2*n*n);
    //find the path
    fastest_path(n,path,maze,map,pos_ball,pos_dest);
    //print the outcome
    for (int index = 0; index <2*n*n&& path[index]!=-1;index++){
        printf("%d",path[index]);
    }
    return 0;
}