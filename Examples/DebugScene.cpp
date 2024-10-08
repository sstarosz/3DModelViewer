#include <print>

#include <Eigen/Core>

void printEigenMatrix4fColumnMajor(const Eigen::Matrix4f& matrix)
{
    for (int row = 0; row < 4; row++)
    {
        std::print("[");
        for (int col = 0; col < 4; col++)   
        {
            std::print("{}", matrix(row, col));
            if (row < 3)
            {
                std::print(", ");
            }
        }
        std::print("]\n");
    }
}


void printEigenMatrix4fRowMajor(const Eigen::Matrix4f& matrix)
{
    for (int row = 0; row < 4; row++)
    {
        std::print("[");
        for (int col = 0; col < 4; col++)
        {
            std::print("{}", matrix(row, col));
            if (col < 3)
            {
                std::print(", ");
            }
        }
        std::print("]\n");
    }
}




int main()
{
    std::print("Debug Scene\n");

    Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Zero();
    viewMatrix(0, 0) = 1.0;
    viewMatrix(0, 1) = 2.0;
    viewMatrix(0, 2) = 3.0;
    viewMatrix(0, 3) = 4.0;
    viewMatrix(1, 0) = 5.0;
    viewMatrix(1, 1) = 6.0;
    viewMatrix(1, 2) = 7.0;
    viewMatrix(1, 3) = 8.0;
    viewMatrix(2, 0) = 9.0;
    viewMatrix(2, 1) = 10.0;
    viewMatrix(2, 2) = 11.0;
    viewMatrix(2, 3) = 12.0;
    viewMatrix(3, 0) = 13.0;
    viewMatrix(3, 1) = 14.0;
    viewMatrix(3, 2) = 15.0;
    viewMatrix(3, 3) = 16.0;

    //viewMatrix.data()[0] = 1.0;
    //viewMatrix.data()[1] = 2.0;
    //viewMatrix.data()[2] = 3.0;
    //viewMatrix.data()[3] = 4.0;
    //viewMatrix.data()[4] = 5.0;
    //viewMatrix.data()[5] = 6.0;
    //viewMatrix.data()[6] = 7.0;
    //viewMatrix.data()[7] = 8.0;
    //viewMatrix.data()[8] = 9.0;
    //viewMatrix.data()[9] = 10.0;
    //viewMatrix.data()[10] = 11.0;
    //viewMatrix.data()[11] = 12.0;
    //viewMatrix.data()[12] = 13.0;
    //viewMatrix.data()[13] = 14.0;
    //viewMatrix.data()[14] = 15.0;
    //viewMatrix.data()[15] = 16.0;

    printEigenMatrix4fColumnMajor(viewMatrix);
    std::println();
    printEigenMatrix4fRowMajor(viewMatrix);


    if(Eigen::Matrix4f::IsRowMajor)
    {
        std::print("Eigen::Matrix4f is row major\n");
    }
    else
    {
        std::print("Eigen::Matrix4f is column major\n");
    }

    
    return 0;
}