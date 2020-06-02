#include<stdio.h>
#include<math.h>
float reynolds_number(float diameter,double velocity,double density,float viscousity);
float factor(float diameter,double velocity,double density,float viscousity,float roughness);
float pressure(float diameter,double velocity,double density,float viscousity,float roughness,double length);
void createfile(float diameter,double velocity,double density,float viscousity,float roughness,double length);
int main(){
    //variable declaration
    float pipe_diameter;
    double pipe_length;
    double fluid_velocity;
    double fluid_density;
    float fluid_viscousity;
    int user_input_number;

    //convert into metres, multiply by 0.001
    float carbon_or_steel = 0.045*0.001;
    float ordinary_concrete = 1*0.001;
    float glass_tube = 0.0015*0.001;
    float pvc_or_plastic = 0.0060*0.001;
    float value_to_use;
    char regime[30];

    //read values from file
    FILE *f = fopen("input.txt","r");
    if(f==NULL){
        printf("Null");
    }else
    {
        fscanf(f,"%f%lf%lf%lf%f",&pipe_diameter,&pipe_length,&fluid_velocity,&fluid_density,&fluid_viscousity);
    }
    //user to select pipe material
     printf("1. Commercial carbon or stainless steel\n");
    printf("2. Ordinary concrete\n");
    printf("3. Glass tube\n");
    printf("4. PVC / plastic\n");
    printf("Enter the number of pipe material you want:");
    scanf("%d",&user_input_number);
    //convert into metres

    //check the number entered and assign value to use to the appropriate material
    /*1 = carbon or steel
    2 = ordinaryconcrete
    3 = glass tube
    4 = pvc or glass*/
    if(user_input_number==1){
        pipe_diameter = 0.76;
        pipe_length = 560000.00;
        fluid_velocity = 1.00;
        fluid_density = 1000.00;
        fluid_viscousity = 0.001;
        value_to_use = carbon_or_steel;
    }else if (user_input_number==2)
    {
        value_to_use =ordinary_concrete;
    }else if (user_input_number==3)
    {
        value_to_use = glass_tube;
    }else if (user_input_number==4)
    {
        value_to_use = pvc_or_plastic;
    }else
    {
        printf("Ivalid choice");
    }
         //caalling functions
       float rey = reynolds_number(pipe_diameter,fluid_velocity,fluid_density,fluid_viscousity);
        factor(pipe_diameter,fluid_velocity,fluid_density,fluid_viscousity,value_to_use);
        float p = pressure(pipe_diameter,fluid_velocity,fluid_density,fluid_viscousity,value_to_use,pipe_length);
        createfile(pipe_diameter,fluid_velocity,fluid_density,fluid_viscousity,value_to_use,pipe_length);
        if (rey<=2100)
        {
            char regime[30] = "laminar";
        }else if (rey>4000)
        {
            char regime[30] = "turbulent";
        }else
        {
            char regime[30] = "transitional";
        }
        //create csv file
        
        
        


        printf("%.2e\n%.2e\n%s\n",rey,p,regime);



}


//functions to calculate reynolds number & frictional pressure
float reynolds_number(float diameter,double velocity,double density,float viscousity){
    float reynolds = (diameter*velocity*density)/viscousity;
    //printf("%e\n",reynolds);
    return reynolds;

}
//calculate factor
float factor(float diameter,double velocity,double density,float viscousity,float roughness){
    float r = reynolds_number(diameter,velocity,density,viscousity);
    float factor;
    //simply the turbulent factor equation
    float ed37 = roughness/(3.7*diameter);
    float ed37power = pow(ed37,1.11);
    //add 6.9 to ed37power
    float add69 = (6.9/r)+ed37power;
    //multiply by -1.8
    float multiply_by18 = -1.8*(log10(add69));
    float laminar_friction;
    float turbulent_friction;
    
    float c = log10(2);

    if (r<=2100){
        factor = 64/r;
        laminar_friction = factor;
    }else if (r>4000)
    {
        factor = pow(multiply_by18,-2);
        turbulent_friction = factor;
    }else if (r>2100 && r<4000)
    {
        factor = laminar_friction+((turbulent_friction-laminar_friction)*((r-2100)/1900));
    }
    


    return factor;

}
//calculate frictional pressure
float pressure(float diameter,double velocity,double density,float viscousity,float roughness,double length){
    //assign moody factor to a variable
    float moody_factor = factor(diameter,velocity,density,viscousity,roughness);
    float z = (length/diameter)*(0.5*density)*(velocity*velocity);
    return moody_factor*z;
}
    
void createfile(float diameter,double velocity,double density,float viscousity,float roughness,double length){
    FILE *fp = fopen("ouput.csv","w");
    for(double i =0.1;i<2.0;i+=0.1){
        fprintf(fp,"%lf\n%.2e",i,pressure(diameter,i,density,viscousity,roughness,length));
    }
    fclose(fp);
    
}
