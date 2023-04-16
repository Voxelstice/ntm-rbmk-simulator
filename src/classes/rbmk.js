class RBMK
{
    constructor(width, height, renderer, statsRenderer, columns) {
        this.width = width
        this.height = height
        this.renderer = renderer
        this.statsRenderer = statsRenderer
        this.columns = columns

        this.consoleImg = new Image()
        this.consoleImg.src = "assets/gui_rbmk_console.png"

        this.reaSim = false
    }

    update(ticks) {
        this.columns.forEach(column => {
            if (column != null) {
                column.update2(ticks, this) // heat spread
                column.update(ticks, this)
            }
        })
    }

    meltdown() {
        options.simulating = false
        document.getElementById("explosionText").style.visibility = "visible"
    }

    draw(ticks) {
        this.renderer.reset()
        this.statsRenderer.reset()

        for (let y = 0; y < this.height; y++) {
            for (let x = 0; x < this.width; x++) {
                var column = this.columns[x + this.width * y]
                // For sprite sheet stuff
                var columnX = 140
                var columnY = 172

                if (column != null) {
                    columnX = column.index * 10
                    column.display.draw(x, y, rbmk)

                    var rect = this.renderer.canvas.canvas.getBoundingClientRect()
                    if (mPos[0]-rect.x > (x*32) && mPos[1]-rect.y > (y*32) && mPos[0]-rect.x < (x*32)+32 && mPos[1]-rect.y < (y*32)+32 && column.tooltip == true) {
                        tooltip.innerHTML = column.tooltipText
                        tooltip.style.visibility = "visible"
                    }

                    if (options.config.columnIndexSelected == x + this.width * y) {
                        this.statsRenderer.draw("image", {
                            img: this.consoleImg,
                            crop: true,
                
                            x: x*32,
                            y: y*32,
                            w: 32,
                            h: 32,
                
                            sX: 0,
                            sY: 192,
                            sW: 10,
                            sH: 10
                        })
                    }
                }

                this.renderer.draw("image", {
                    img: this.consoleImg,
                    crop: true,

                    x: x*32,
                    y: y*32,
                    w: 32,
                    h: 32,

                    sX: columnX,
                    sY: columnY,
                    sW: 10,
                    sH: 10
                }) 
            }
        }

        this.columns.forEach(column => {
            if (column != null) {
                column.draw(ticks)
            }
        })
    }

    stopSimulation() {
        options.frames = 0
        document.getElementById("explosionText").style.visibility = "hidden"
        this.columns.forEach(column => {
            if (column != null) {
                column.heat = 20
                column.rs_steam = 0
                column.rs_water = 0

                column.reset()
            }
        })
    }
}