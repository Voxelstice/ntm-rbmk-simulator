class Display
{
    constructor(displayIndex, column) {
        this.displayIndex = displayIndex * 10
        this.column = column
    }

    draw(x, y, rbmk) {
        // The heat will always render
        var h = quickMath(this.column.heat - 20, 10, this.column.maxHeat)
        var h2 = quickMath(this.column.heat - 20, 32, this.column.maxHeat)
        rbmk.statsRenderer.draw("image", {
            img: rbmk.consoleImg,
            crop: true,

            x: x*32,
            y: y*32+32-h2,
            w: 32,
            h: h2,

            sX: 0,
            sY: 182+(10-h),
            sW: 10,
            sH: h
        })

        // Other
        this.draw2(x, y, rbmk)
    }

    draw2(x, y, rbmk) {
        rbmk.statsRenderer.draw("image", {
            img: rbmk.consoleImg,
            crop: true,

            x: x*32,
            y: y*32,
            w: 32,
            h: 32,

            sX: this.displayIndex,
            sY: 182,
            sW: 10,
            sH: 10
        })
    }
}