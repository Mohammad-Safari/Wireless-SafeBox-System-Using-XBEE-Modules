unsigned int a = 0;

void setup()
{
    Serial.begin(9600);

    a = 2;

    Serial.print("a = ");
    Serial.println(a);

    asm("mov r0,%0 \n\t" // copy a to r0
        "inc r0    \n\t" // increment
        "inc r0    \n\t" // increment
        "mov %0,r0 \n\t" // copy r0 back to a
        : "+r"(a));

    Serial.print("now a = ");
    Serial.println(a);
}

void loop()
{
}