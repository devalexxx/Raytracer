
namespace rtc
{

	template<typename T>
	Shape::Shape(T shape) :
		Intersect([&, this](const Ray& ray) {
			return rtc::Intersect(ray, *this, shape);
		})
	{}

}