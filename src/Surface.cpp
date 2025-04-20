#include "Surface.hpp"
#include "Game.hpp"
#include "RoundedRectangleShape.hpp"
#include "UveDX.hpp"
#include <format>

namespace UveDX {
Surface::Surface(UveDX *uveDX)
    : UveBase(uveDX), nextSurface(this), width(0), height(0), offsetX(0),
      offsetY(0), field28(1), scaleFactor(1.0), texture(nullptr),
      sprite(nullptr) {}

Surface::Surface(UveDX *uveDX, const std::string &filename) : Surface(uveDX) {
  this->uveDX->log(std::format("Loading file [{}]", filename));

  unsigned int fileSize = 0;

  auto fileStream = this->uveDX->uveFileManager->openFile(filename, &fileSize);

  auto imageFile = new char[fileSize];

  fileStream.read(imageFile, fileSize);

  sf::Image image;

  bool success = image.loadFromMemory(imageFile, fileSize);

  if (!success) {
    delete[] imageFile;

    this->uveDX->onError("Surface::Surface()", "Cannot create surface");

    return;
  }

  image.createMaskFromColor(sf::Color::Black);

  this->texture = new sf::Texture{image};
  this->sprite = new sf::Sprite{*this->texture};

  this->width = this->texture->getSize().x;
  this->height = this->texture->getSize().y;

  delete[] imageFile;

  this->setAnchorPoint(SurfaceAnchorType::Center);
}

Surface::~Surface() {
  if (this->texture)
    delete this->texture;

  if (this->sprite)
    delete this->sprite;
}

void Surface::update() {}

void Surface::blit(int dstX, int dstY, const sf::IntRect *srcRect,
                   double scale) {
  if (!this->texture || !this->sprite)
    return;

  sf::IntRect textureRect;

  if (srcRect == nullptr)
    textureRect =
        sf::IntRect({0, 0}, {static_cast<int>(this->texture->getSize().x),
                             static_cast<int>(this->texture->getSize().y)});
  else
    textureRect = *srcRect;

  this->sprite->setTextureRect(textureRect);

  this->sprite->setPosition({
      static_cast<float>(dstX - this->offsetX),
      static_cast<float>(dstY - this->offsetY),
  });

  this->sprite->setScale({
      static_cast<float>(scale),
      static_cast<float>(scale),
  });

  global::game->window.draw(*this->sprite);
}

void Surface::blitWithColor(sf::IntRect *rect, sf::Color color) {
  sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(this->width),
                                            static_cast<float>(this->height)));
  rectangle.setPosition({0.0f, 0.0f});
  rectangle.setFillColor(color);

  if (rect) {
    rectangle.setSize(sf::Vector2f(rect->size.x, rect->size.y));
    rectangle.setPosition(sf::Vector2f(static_cast<float>(rect->position.x),
                                       static_cast<float>(rect->position.y)));
  }

  global::game->window.draw(rectangle);
}

void Surface::createAsBackSurface() {
  this->width = this->uveDX->width;
  this->height = this->uveDX->height;

  this->blitWithColor();
}

void Surface::loadBMP(const std::string &filename, unsigned int x,
                      unsigned int y) {
  this->uveDX->log(std::format("Loading BPM onto surface [{}].", filename));

  unsigned int fileSize = 0;

  auto fileStream = this->uveDX->uveFileManager->openFile(filename, &fileSize);

  auto imageFile = new char[fileSize];

  fileStream.read(imageFile, fileSize);

  auto texture = sf::Texture{imageFile, fileSize};

  delete[] imageFile;

  if (x == 0xFFFF8000)
    x = (this->width - texture.getSize().x) / 2;

  if (y == 0xFFFF8000)
    y = (this->height - texture.getSize().y) / 2;

  sf::Sprite sprite{texture};

  sprite.setPosition(
      sf::Vector2f{static_cast<float>(x), static_cast<float>(y)});

  global::game->window.draw(sprite);
}

void Surface::drawDebugShape(int x, int y) {
  int halfWidth = this->width / 2;
  int halfHeight = this->height / 2;

  int adjustedX = halfWidth + x - this->offsetX;
  int adjustedY = halfHeight + y - this->offsetY;

  int scaledWidth = static_cast<int>(halfWidth * this->scaleFactor);
  int scaledHeight = static_cast<int>(halfHeight * this->scaleFactor);

  int cornerRadius = std::min(scaledWidth, scaledHeight);

  sf::Vector2f shapeSize{static_cast<float>(scaledWidth * 2),
                         static_cast<float>(scaledHeight * 2)};

  sf::RoundedRectangleShape roundedRect{shapeSize,
                                        static_cast<float>(cornerRadius), 4};

  roundedRect.setPosition({static_cast<float>(adjustedX - scaledWidth),
                           static_cast<float>(adjustedY - scaledHeight)});
  roundedRect.setFillColor(sf::Color::Transparent);
  roundedRect.setOutlineColor(sf::Color::White);
  roundedRect.setOutlineThickness(1.f);

  global::game->window.draw(roundedRect);
}

int Surface::getWidth() const { return this->width; }

int Surface::getHeight() const { return this->height; }

int Surface::getOffsetX() const { return this->offsetX; }

int Surface::getOffsetY() const { return this->offsetY; }

double Surface::getScaleFactor() const { return this->scaleFactor; }

int Surface::getField28() const { return this->field28; }

Surface *Surface::getNextSurface() { return this->nextSurface; }

void Surface::setScaleFactor(double scale) { this->scaleFactor = scale; }

void Surface::setField28(int val) { this->field28 = val; }

void Surface::setAnchorPoint(SurfaceAnchorType anchorType) {
  switch (anchorType) {
  case SurfaceAnchorType::TopLeftDefault:
  case SurfaceAnchorType::TopLeft:
    this->offsetX = 0;
    this->offsetY = 0;
    break;
  case SurfaceAnchorType::BottomLeft:
    this->offsetX = 0;
    this->offsetY = this->height;
    break;
  case SurfaceAnchorType::BottomCenter:
    this->offsetX = this->width / 2;
    this->offsetY = this->height;
    break;
  case SurfaceAnchorType::BottomRight:
    this->offsetX = this->width;
    this->offsetY = this->height;
    break;
  case SurfaceAnchorType::CenterLeft:
    this->offsetX = 0;
    this->offsetY = this->height / 2;
    break;
  case SurfaceAnchorType::Center:
    this->offsetX = this->width / 2;
    this->offsetY = this->height / 2;
    break;
  case SurfaceAnchorType::CenterRight:
    this->offsetX = this->width;
    this->offsetY = this->height / 2;
    break;
  case SurfaceAnchorType::TopCenter:
    this->offsetX = this->width / 2;
    this->offsetY = 0;
    break;
  case SurfaceAnchorType::TopRight:
    this->offsetX = this->width;
    this->offsetY = 0;
    break;
  default:
    return;
  }
}

void Surface::setNextSurface(Surface *nextSurface) {
  this->nextSurface = nextSurface;
}
} // namespace UveDX
