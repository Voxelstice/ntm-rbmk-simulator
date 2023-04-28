class Renderer
{
    constructor(canvas) {
        this.canvasObj = canvas
        this.canvas = canvas.getContext("2d")
        this.canvas.imageSmoothingEnabled = false
    }

    draw(type, args) {
        if (type == "image") {
            /*
                args:
                    - img: Image
                    
                    - x: int // X position of the image
                    - y: int // Y position of the image
                    - w: int // Width of the image
                    - h: int // Height of the image

                    / crop: true
                    - sX: int // X position of the cropped image
                    - sY: int // Y position of the cropped image
                    - sW: int // Width of the cropped image
                    - sH: int // Height of the cropped image
            */

            if (args.crop) {
                this.canvas.drawImage(args.img, args.sX, args.sY, args.sW, args.sH, args.x, args.y, args.w, args.h)
            } else if (args.crop == false) {
                this.canvas.drawImage(args.img, args.x, args.y, args.w, args.h)
            }
        }
    }

    reset() {
        this.canvas.imageSmoothingEnabled = false
        this.canvas.clearRect(0, 0, this.canvas.canvas.width, this.canvas.canvas.height);
    }
}
