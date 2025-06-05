using System;

class Math
{
    public void PrintSum(params int[] numbers)
    {
        int sum = 0;
        foreach (var n in numbers)
            sum += n;
        Console.WriteLine(sum);
    }
}

class Program
{
    static void Main(string[] args)
    {
        var objMath = new Math();   // Se coloca un breakpoint aquí
        // Ejecución se pausa, se puede continuar con steps.
        objMath.PrintSum(10, 20, 30, 40, 50);
    }
}