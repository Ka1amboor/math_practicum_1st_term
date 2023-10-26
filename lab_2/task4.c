#include <stdio.h>
#include <stdarg.h>

enum STATUS_CODE
{
    SUCCESS = 0,
    WRONG = 1,
    NOT_POLYGON = 2,
    INVALID_VALUE = 3
};

enum STATUS_CODE polynomial_value(double *result, double x, int count, ... )
{
    va_list ptr;
    if (count > 0)
    {
        va_start(ptr, count);
        for (int i = 0; i <= count; i++)
        {
            *result = *result * x + va_arg(ptr, double);
        }
    }
    else
    {   if (x == 0.0)
        {
            return INVALID_VALUE;
        }

        va_start(ptr, count);
        for (int i = 0; i >= count; i--)
        {
            *result = *result / x + va_arg(ptr, double);
        }

    }
    va_end(ptr);
    return SUCCESS;
}


typedef struct 
{
    double x;
    double y;

} Point;

double z_cross_components(Point p1, Point p2, Point p3)
{
    double x1 = p2.x - p1.x;
    double y1 = p2.y - p1.y;
    double x2 = p3.x - p2.x;
    double y2 = p3.y - p2.y;

    return ((x1 * y2) - (y1 * x2));
}

enum STATUS_CODE check_convex(int count, ...)
{   
    double EPSILON = 0.000001;
    if (count < 3)
    {
        return NOT_POLYGON;
    }
    va_list args;
    va_start(args, count);
    
    Point p[count];
    for (int i = 0; i < count; i++)
    {
        p[i] = va_arg(args, Point);
    }
    va_end(args);

    double res_z_cross_components = 0;
    int positive = 0;
    int negative = 0;

    for (int k = 0; k < count; k++)
    {
        int k1 = (k + 1) % count;
        int k2 = (k + 2) % count;

        res_z_cross_components = z_cross_components(p[k], p[k1], p[k2]);

        if (res_z_cross_components > EPSILON)
        {
            positive = 1;
        }
        else if (res_z_cross_components < (EPSILON * (-1.0)))
        {
            negative = 1;
        }

        if (positive && negative)
        {
            return WRONG; //not convex
        }
    }
    return SUCCESS; //is convex
}

int main()
{   
    double result = 0;
    double x = 1.0;
    int count = -3;
    double a[] = {2.0, -3.0, 4.0, -5.0};

    enum STATUS_CODE result_polynom = polynomial_value(&result, x, count, a[0], a[1], a[2], a[3]);
    
    if (result_polynom == INVALID_VALUE)
    {
        printf("Invalid value!\n");
    }
    if (result_polynom == SUCCESS)
    {
        printf("The value of polynom in point %f: %f\n", x, result);
    }



    Point points[5] = {{1, 1}, {4, 5}, {4, 3}, {8, 4}, {3, -1}};

    if (check_convex(5, points[0], points[1], points[2], points[3], points[4]) == NOT_POLYGON)
    {
        printf("This shape can't be a polygon");
    }
    else if (check_convex(5, points[0], points[1], points[2], points[3], points[4]) == WRONG)
    {
        printf("No, this polygon is not convex");
    }
    else
    {
        printf("Yes, this polygon is convex");
    }

    return 0;
}
