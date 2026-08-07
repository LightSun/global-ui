#include <iostream>
#include <gtest/gtest.h>

using namespace std;

//#define H7_TEST_GUI 1

namespace taichi {

extern void main_test_gui();
//
static inline void h7_test_gui(){
    main_test_gui();
}

}

int main(int argc, char **argv) {
#ifdef H7_TEST_GUI
    taichi::h7_test_gui();
    return 0;
#else
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
// 1. 定义一个测试夹具类
class VectorTest : public ::testing::Test {
protected:
    // 2. 在 SetUp() 中初始化数据
    void SetUp() override {
        v0_.push_back(1);
        v0_.push_back(2);
    }

    // void TearDown() override {} // 如果需要清理，可以重写此函数

    // 3. 声明共享数据
    std::vector<int> v0_;
    std::vector<int> v1_; // 初始为空
};

// 4. 使用 TEST_F 编写测试
TEST_F(VectorTest, IsEmptyInitially) {
    EXPECT_TRUE(v1_.empty());
}

TEST_F(VectorTest, SizeAfterPush) {
    v1_.push_back(3);
    EXPECT_EQ(1, v1_.size());
    // v0_ 在这里仍然保有在 SetUp 中初始化的数据 {1, 2}
    EXPECT_EQ(2, v0_.size());
}
