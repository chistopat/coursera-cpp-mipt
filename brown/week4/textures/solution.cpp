#include "Common.h"
#include <functional>
#include <optional>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

namespace Shapes {
    class BaseShape : public IShape {
    public:
        const char kDefaultPixel = '.';

    public:
        explicit BaseShape() {}

        BaseShape(const BaseShape& other) {
            SetPosition(other.position_);
            SetTexture(other.texture_);
            SetSize(other.size_);
        }

        virtual ~BaseShape() = default;

        void SetPosition(Point point) final {
            position_ = move(point);
        }

        Point GetPosition() const final {
            return position_;
        }

        void SetSize(Size size) final {
            size_ = move(size);
        }

        Size GetSize() const final {
            return size_;
        }

        void SetTexture(std::shared_ptr<ITexture> texture) final {
            texture_ = move(texture);
        }

        ITexture * GetTexture() const final {
            return texture_.get();
        }

    public:
        virtual void Draw(Image &) const = 0;
        virtual std::unique_ptr<IShape> Clone() const = 0;

    public:
        bool is_pixel_on_shape(int y, int x) const {
            if (y >= GetPosition().y && y < GetPosition().y + GetSize().height) {
                if (x >= GetPosition().x && x < GetPosition().x + GetSize().width) {
                    return true;
                }
            }
            return false;
        }

    private:
        Point position_;
        Size size_;
        std::shared_ptr<ITexture> texture_;


    };

    class Rectangle : public BaseShape {
    public:
        using ShapeFilter = std::function<bool (Point, Size)>;
    public:
        void Draw(Image& image) const override{
            return Draw(image, [](Point p, Size s) {return true;});
        }

        std::unique_ptr<IShape> Clone() const override {
            return make_unique<Rectangle>(*this);
        }


        void Draw(Image& image,  ShapeFilter shape_filter) const {
            for (int global_y = 0; global_y < static_cast<int>(image.size()); ++global_y) {
                for (int global_x = 0; global_x < static_cast<int>(image[global_y].size()); ++global_x ) {
                    if (is_pixel_on_shape(global_y, global_x)) {
                        auto pixel = GetPixel(global_y, global_x, shape_filter);
                        if (pixel.has_value()) {
                            image[global_y][global_x] = pixel.value();
                        }
                    }
                }
            }
        }

        std::optional<char> GetPixel(int global_y, int global_x, ShapeFilter shape_filter) const {
            const auto texture = GetTexture();
            int shape_y = global_y - GetPosition().y;
            int shape_x = global_x - GetPosition().x;
            if (shape_filter({shape_x, shape_y}, GetSize())) {
                if (!texture
                    || shape_y >= static_cast<int>(texture->GetImage().size())
                    || shape_x >= static_cast<int>(texture->GetImage()[shape_y].size())) {
                    return BaseShape::kDefaultPixel;
                }
                return texture->GetImage()[shape_y][shape_x];
            }
            return {};
        }
    };

    class Ellipse : public Rectangle {
    public:
        void Draw(Image &image) const override {
            return Rectangle::Draw(image, [](Point p, Size s) {return IsPointInEllipse(p, s);});
        }
        std::unique_ptr<IShape> Clone() const override {
            return make_unique<Ellipse>(*this);
        }
    };
}

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    using namespace Shapes;
    switch(shape_type) {
        case ShapeType::Rectangle:
            return make_unique<Rectangle>();
        case ShapeType::Ellipse:
            return make_unique<Ellipse>();
    }
}