# 2D游戏引擎 | 物理引擎篇：Body

​	先完成游戏引擎中的物理引擎，物理引擎提供真实的物体运动。其中最重要的部分是物体(Body)，接下来介绍2DGEBody代码构成以及如何使用。

![](http://beisent.com/img/blog/cpp/2dGE/2DGEBody.png)

使用枚举类型(Shape)来存储Body的类型。

Body包含圆形(Circle)，多边形(Polygon)，无形状(NONE)后续可能会更新更多图形。

```C++
enum Shape {
	NONE = -1,
	CIRCLE = 0,
	POLTGON = 1
};
```

使用结构体来存储Body的颜色后续可能会更新材质(texture)属性

```C++
struct BodyColor
{
	int r;
	int g;
	int b;//颜色RGB
	int a;//透明度
};
```

Body类型，只包含了基础属性，每个物体都有自己的id

```c++
class Body {
public:
	int body_id_ = 0;
	Shape shape_ = NONE;
	float radius_ = 0;
	float mass_ = 0;
	BodyColor color_ = { 0,0,0,0 };
	FlatVector mass_center_ = { 0,0 };
	std::vector<SDL_FPoint> vertices_ = {};

	Body();
	~Body();
	Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id);
	Body(Shape shape, std::vector<SDL_FPoint> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id);

	void Move(FlatVector v1);//用于在原来位置上移动v1
	void MoveTo(FlatVector v1);//直接位移到v1

	void Rotation(float angle);//绕质心旋转angle°
	
};
```

BodyManager类型，用于管理Body

```C++
class BodyManager {
public:
	BodyManager();
	~BodyManager();

	std::vector<Body> body_lists_;//存放Body类型
	int id_count = 0;
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center);
	bool CreateBody(std::vector<SDL_FPoint> vertices, BodyColor color, float mass);
	
	bool DestroyBody(int body_id);//销毁物体
	std::vector<Body>::iterator FindBody(int body_id);//查找物体，找到返回对应迭代器，没找到返回end迭代器
	void RenderBody(Brush& brush);//渲染物体，相当于更新物体的状态
	void CoutBodyList();//打印Bodylist
	
}
```

还有两个全局函数用于获取物体的质心

```C++
FlatVector GetMassCenter(Body& body);
FlatVector GetMassCenter(std::vector<SDL_FPoint> points);
```

