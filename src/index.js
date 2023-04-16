const rbmkCanvas = document.getElementById("rbmk")
const statsCanvas = document.getElementById("stats")

// A couple of functions
function quickMath(val, mult, div) {
	return Math.min(Math.ceil( val * mult / div ), mult)
}
function fillArray(array, amount) {
    var fakeArray = array
    for (let i = 0; i < amount; i++) {
        fakeArray[i] = null
    }
    return fakeArray
}
function findConstructor(array, name) {
    var constructor
    array.forEach(ah => {
        var thing = new ah()
        if (thing.constructor.name == name) {
            constructor = ah
        }
    })
    return constructor
}
function averageCalc(array) {
    var number = 0
    for (item of array) {
        number += item
    }
    var result = number / array.length
    if (isNaN(result) == true) {
        return 0
    } else {
        return result
    }
}

/// TOOLTIP ///
var mPos = [0,0]
var tooltip = document.createElement('div')
tooltip.innerHTML = 'Hello World'
tooltip.className = "tooltip"
document.body.appendChild(tooltip)

document.body.addEventListener("mousemove", (e) => {
    mPos = [e.clientX, e.clientY]
})

/// BUTTON ///
var btnFuncs = {
    ["clear"]: function(btn) {
        rbmk.columns = fillArray([], 15*15)
        rbmk.columns[112] = new Blank()
    },
    ["run"]: function(btn) {
        if (options.simulating == false) {
            if (btn.innerHTML == "Stop") {
                rbmk.stopSimulation()
                btn.innerHTML = "Run"
            } else {
                options.az5 = false
                rbmk.stopSimulation() // This just resets properties to their defaults, again
                options.simulating = true
                btn.innerHTML = "Stop"
            }
        } else if (options.simulating == true) {
            options.simulating = false
            rbmk.stopSimulation()
            btn.innerHTML = "Run"
        }
    },
    ["placeconfig"]: function(btn) {
        if (options.place.placing == false) {
            options.place.placing = true
            btn.innerHTML = "Place"
            options.config.columnIndexSelected = -1
        } else if (options.place.placing == true) {
            options.place.placing = false
            btn.innerHTML = "Config"
        }
    },
    ["pullcrs"]: function(btn) {
        var res = prompt("Target level")

        // Do math
        var num = Number.parseInt(res)
        if (num < 0) {
            num = 0
        }
        if (num > 100) {
            num = 100
        }
        num = num/100
        if (isNaN(num) == true) {
            num = 0
        }

        // Do stuff
        rbmk.columns.forEach(column => {
            if (column instanceof Control) {
                column.targetLevel = num
            }
        })
    },
    ["az5"]: function(btn) {
        if (options.az5 == false) {
            options.az5 = true

            options.az5snd.volume = 0.4
            options.az5snd.pause()
            options.az5snd.currentTime = 0
            options.az5snd.play()

            rbmk.columns.forEach(column => {
                if (column instanceof Control) {
                    column.targetLevel = 0
                }
            })
        }
    },
    ["boilerInputRate"]: function() {
        var res = prompt("Boiler input rate")
        var num = Number.parseInt(res)
        if (num < 0) {
            num = 0
        }
        if (num > 16000) {
            num = 16000
        }
        if (isNaN(num) == true) {
            num = 0
        }

        options.rbmkStuff.boilerInputRate = num
    },
    ["boilerOutputRate"]: function() {
        var res = prompt("Boiler output rate")
        var num = Number.parseInt(res)
        if (num < 0) {
            num = 0
        }
        if (num > 16000) {
            num = 16000
        }
        if (isNaN(num) == true) {
            num = 0
        }

        options.rbmkStuff.boilerOutputRate = num
    },
    ["reasimBoilers"]: function() {
        var res = confirm("Reasim boilers?")
        RBMKDials.dialReasimBoilers = res
    },
    ["export"]: function() {
        if (options.simulating == true) {
            return alert("You can't export while simulating")
        }
        var json = {"version": "rbmk_1", "data": [], "rbmk": {"rbmkdials": {}, "boilerInputRate": 0, "boilerOutputRate": 0}}

        var index = 0
        rbmk.columns.forEach(column => {
            json.data.push({"class": column != null ? column.constructor.name.toLowerCase() : null, "variables": {}})

            for (const property in column) {
                if (typeof(column[property]) != "object") {
                    json.data[index].variables[property] = column[property]
                } else if (typeof(column[property]) == "object") {
                    if (property == "fuel") {
                        json.data[index].variables[property] = {"construct": column[property].constructor.name}
                    }
                }
            }

            index += 1
        })

        json.rbmk.boilerInputRate = options.rbmkStuff.boilerInputRate
        json.rbmk.boilerOutputRate = options.rbmkStuff.boilerOutputRate
        for (const property in RBMKDials) {
            json.rbmk.rbmkdials[property] = RBMKDials[property]
        }

        try {
            navigator.clipboard.writeText(JSON.stringify(json))
            alert("Export successful")
        } catch (err) {
            alert(err)
        }
    },
    ["import"]: function() {
        if (options.simulating == true) {
            return alert("You can't import while simulating")
        }
        var res = prompt("Enter json data")
        try {
            var json = JSON.parse(res)
            var rbmkColumnData = []
    
            if (!json.version) {
                alert("This data doesn't contain version")
                return
            }
            if (!json.data) {
                alert("This data doesn't contain any data")
                return
            }
    
            if (json.version != "rbmk_1") {
                alert(`This data is not up-to-date (${json.version}), expect some issues`)
            }
    
            var index = 0
            json.data.forEach(element => {
                rbmkColumnData[index] = getConstructor(element.class)

                for (const property in element.variables) {
                    if (element.variables[property].construct) {
                        var ah = findConstructor(fuels, element.variables[property].construct)
                        rbmkColumnData[index][property] = new ah()
                    } else {
                        rbmkColumnData[index][property] = element.variables[property]
                    }
                }
    
                index += 1
            })

            options.rbmkStuff.boilerInputRate = json.rbmk.boilerInputRate
            options.rbmkStuff.boilerOutputRate = json.rbmk.boilerOutputRate
            for (const property in json.rbmk.rbmkdials) {
                RBMKDials[property] = json.rbmk.rbmkdials[property]
            }
    
            rbmk.columns = fillArray([], 15*15)
            rbmk.columns = rbmkColumnData
        } catch (err) {
            alert(`Error occurred: ${err}`)
            console.trace(err)
        }
    }
}

function button() {
    var btn = document.elementFromPoint(mPos[0], mPos[1])
    if (btnFuncs[btn.getAttribute("action")]) {
        btnFuncs[btn.getAttribute("action")](btn)
    }
    options.config.prevColumn = 0
}

// Main script
var options = {
    // RBMK itself
    az5: false,
    az5snd: new Audio("https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/resources/assets/hbm/sounds/block/shutdown.ogg?raw=true"),
    simulating: false,
    frames: 0,

    rbmkStuff: {
        boilerInputRate: 0,
        boilerOutputRate: 0
    },

    // Placement
    place: {
        placing: true,
        selected: "blank",
        blocks: [
            {
                select: "blank",
                index: 0
            },
            {
                select: "fuel",
                index: 1
            },
            {
                select: "control",
                index: 2
            },
            /*{
                select: "control_auto",
                index: 3
            },*/
            {
                select: "boiler",
                index: 4
            },
            {
                select: "moderator",
                index: 5
            },
            {
                select: "absorber",
                index: 6
            },
            {
                select: "reflector",
                index: 7
            },
            /*{
                select: "outgasser",
                index: 8
            },*/
            {
                select: "storage",
                index: 11
            },
            {
                select: "cooler",
                index: 12
            },
            /*{
                select: "heatex",
                index: 13
            },*/
            {
                select: "nothing",
                index: 14
            },
        ]
    },
    config: {
        columnIndexSelected: -1,
        prevColumn: 0
    }
}
var configMenu = document.getElementById("config_main")
var toolsRenderer = new Renderer(document.getElementById("tools"))
var renderer = new Renderer(rbmkCanvas)
var statsRenderer = new Renderer(statsCanvas)
var rbmk = new RBMK(15, 15, renderer, statsRenderer, fillArray([], 15*15))

// Create these screen-borders
function createScreen(t, def) {
    var div = document.createElement("div")
    div.className = "buttonBorder"
    div.style.marginTop = "5px"
    div.style.marginBottom = "5px"

    var title = document.createElement("p")
    title.className = "noMargin"
    title.innerHTML = `<b>${t}</b>`

    var value = document.createElement("p")
    value.className = "noMargin"
    value.style.fontSize = "45px"
    value.innerHTML = def

    div.insertAdjacentElement("beforeend", title)
    div.insertAdjacentElement("beforeend", value)

    return {div, title, value}
}

var averageStats = document.getElementById("averageStats")
var averageColumnTemp = createScreen("Column Temp", "20°C")
var controlRodLevel = createScreen("Control Rod Level", "0%")
var fuelDepletion = createScreen("Fuel Depletion", "0%")
var xenonPoisonLevel = createScreen("Xenon Poisoning", "0%")
var coreTempFuel = createScreen("Fuel Temp", "20°C")
var powerProduced = createScreen("Power Produced", "0 HE")

averageStats.insertAdjacentElement("beforeend", averageColumnTemp.div)
averageStats.insertAdjacentElement("beforeend", controlRodLevel.div)
averageStats.insertAdjacentElement("beforeend", fuelDepletion.div)
averageStats.insertAdjacentElement("beforeend", xenonPoisonLevel.div)
averageStats.insertAdjacentElement("beforeend", coreTempFuel.div)
averageStats.insertAdjacentElement("beforeend", powerProduced.div)

// Loop
setInterval(() => {
    tooltip.style.visibility = "hidden"
    tooltip.style.left = (mPos[0]+15) + "px"
    tooltip.style.top = mPos[1] + window.scrollY + "px"

    var hoveringOn = document.elementFromPoint(mPos[0], mPos[1])
    if (hoveringOn) {
        if (hoveringOn.getAttribute("tooltip")) {
            tooltip.innerHTML = hoveringOn.getAttribute("tooltip")
            tooltip.style.visibility = "visible"
        }
    }

    // RBMK
    if (options.simulating == true) {
        options.frames++
        rbmk.update(options.frames)
    }
    rbmk.draw(options.frames)

    // Average stats
    var columnTemps = []
    var controlRodLevels = []
    var fuelDepletions = []
    var xenonPoisons = []
    var coreTempFuels = []
    var powerProducedd = []

    // Get all variables
    rbmk.columns.forEach(column => {
        if (column != null) {
            columnTemps.push(column.heat)

            if (column instanceof Control) {
                controlRodLevels.push(column.level*100)
            }
            if (column instanceof Fuel) {
                fuelDepletions.push(column.depletion)
                xenonPoisons.push(column.xenonPoison)
                coreTempFuels.push(column.coreHeat)
            }
            if (column instanceof Boiler) {
                powerProducedd.push(column.producedPower)
            }
        }
    })

    // Set stuff
    averageColumnTemp.value.innerHTML = `${averageCalc(columnTemps).toFixed(1)}°C`
    controlRodLevel.value.innerHTML = `${averageCalc(controlRodLevels).toFixed(1)}%`
    fuelDepletion.value.innerHTML = `${averageCalc(fuelDepletions).toFixed(3)}%`
    xenonPoisonLevel.value.innerHTML = `${averageCalc(xenonPoisons).toFixed(1)}%`
    coreTempFuel.value.innerHTML = `${averageCalc(coreTempFuels).toFixed(1)}°C`
    if (options.frames % 5 == 0) {
        powerProduced.value.innerHTML = `${averageCalc(powerProducedd).toFixed(1)} HE`
    }

    // Placement
    toolsRenderer.reset()

    var x = 0
    var y = 0
    options.place.blocks.forEach(block => {
        var rect = toolsRenderer.canvas.canvas.getBoundingClientRect()
        if (mPos[0]-rect.x > (x*32) && mPos[1]-rect.y > (y*32) && mPos[0]-rect.x < (x*32)+32 && mPos[1]-rect.y < (y*32)+32) {
            tooltip.innerHTML = `<b>${block.select.toUpperCase()}</b>`
            tooltip.style.visibility = "visible"
        }

        toolsRenderer.draw("image", {
            img: rbmk.consoleImg,
            crop: true,

            x: x*32,
            y: y*32,
            w: 32,
            h: 32,

            sX: block.index * 10,
            sY: 172,
            sW: 10,
            sH: 10
        })
        if (options.place.selected == block.select && options.simulating == false && options.place.placing == true) {
            toolsRenderer.draw("image", {
                img: rbmk.consoleImg,
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
        x++
        if (x >= 14) {
            x = 0
            y++
        }
    })


    // Config
    if (rbmk.columns[options.config.columnIndexSelected] != null && options.config.columnIndexSelected != options.config.prevColumn) {
        configMenu.innerHTML = ""
        var columnName = document.createElement("p")
        columnName.className = "noMargin"
        columnName.style.fontSize = "35px"
        columnName.innerHTML = `<b>${rbmk.columns[options.config.columnIndexSelected].constructor.name} (${options.config.columnIndexSelected})</b>`

        configMenu.insertAdjacentElement("beforeend", columnName)
        var j = rbmk.columns[options.config.columnIndexSelected].getConfig()
        if (typeof(j) == "object") {
            configMenu.insertAdjacentElement("beforeend", j)
        } else {
            configMenu.innerHTML = configMenu.innerHTML + j
        }
    } else if (rbmk.columns[options.config.columnIndexSelected] == null && options.config.columnIndexSelected != options.config.prevColumn) {
        configMenu.innerHTML = ""
        // If nothingness is selected, default to RBMK options        
        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Boilers water input rate: ${options.rbmkStuff.boilerInputRate}`
        elementt.setAttribute("tooltip", "Will also affect coolers")
        elementt.setAttribute("action", "boilerInputRate")
        elementt.setAttribute("onclick", `button()`)

        configMenu.insertAdjacentElement("beforeend", elementt)

        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Boilers steam output rate: ${options.rbmkStuff.boilerOutputRate}`
        elementt.setAttribute("action", "boilerOutputRate")
        elementt.setAttribute("onclick", `button()`)

        configMenu.insertAdjacentElement("beforeend", elementt)

        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `ReaSim boilers: ${RBMKDials.dialReasimBoilers}`
        elementt.setAttribute("action", "reasimBoilers")
        elementt.setAttribute("onclick", `button()`)

        configMenu.insertAdjacentElement("beforeend", elementt)

        var elementt = document.createElement("p")
        elementt.style.fontSize = "27px"
        elementt.innerHTML = "If you want to edit gamerules, please open developer tools and check the console"

        configMenu.insertAdjacentElement("beforeend", elementt)
    }

    options.config.prevColumn = options.config.columnIndexSelected

    // Display buttons when simulating
    var btns = document.getElementsByClassName("showInSim")
    for (let i = 0; i < btns.length; i++) {
        const btn = btns[i]
        btn.style.display = options.simulating ? "unset" : "none"
    }
    var btns = document.getElementsByClassName("showInDesign")
    for (let i = 0; i < btns.length; i++) {
        const btn = btns[i]
        btn.style.display = options.simulating ? "none" : "unset"
    }

    // Specific stuff
    if (options.simulating == true) {
        rbmk.columns.forEach(column => {
            if (column != null) {
                if (RBMKDials.dialReasimBoilers == true) {
                    column.rs_water += options.rbmkStuff.boilerInputRate
                }
            }
            if (column instanceof Boiler) {
                column.feedwater += options.rbmkStuff.boilerInputRate
            }
            if (column instanceof Cooler) {
                column.cryo += options.rbmkStuff.boilerInputRate
            }
        })
    }
}, 20)

function configMenuAction() {
    var btn = document.elementFromPoint(mPos[0], mPos[1])
    var col = rbmk.columns[options.config.columnIndexSelected]
    options.config.prevColumn = -1
    switch (btn.getAttribute("configmenuaction")) {
        case "moderation":
            col.moderated = !col.moderated
            break
        case "pull":
            var res = prompt("Target level")
            // Do math
            var num = Number.parseInt(res)
            if (num < 0) {
                num = 0
            }
            if (num > 100) {
                num = 100
            }
            num = num/100

            col.targetLevel = num
            break
        case "compression":
            col.steamType++
            if (col.steamType > 3) {
                col.steamType = 0
            }
            break
    }
}

// Main placement system
function clamp(val, a, b) {
    if (val >= b) {
        return b
    } else if (val <= a) {
        return a
    } else {
        return val
    }
}
function getConstructor(block) {
    switch (block) {
        // TODO: Make it actually get the right class without this mess
        case "blank":
            return new Blank()
        case "fuel":
            return new Fuel()
        case "control":
            return new Control()
        case "control_auto":
            return new ControlAuto()
        case "boiler":
            return new Boiler()
        case "moderator":
            return new Moderator()
        case "absorber":
            return new Absorber()
        case "reflector":
            return new Reflector()
        case "outgasser":
            return new Outgasser()
        case "storage":
            return new Storage()
        case "cooler":
            return new Cooler()
        case "heatex":
            return new Heatex()
        case "nothing":
            return null
        default:
            return null
    }
}

document.body.addEventListener("click", function() {
    var rect = rbmk.renderer.canvas.canvas.getBoundingClientRect()
    var x = mPos[0]-rect.x
    var y = mPos[1]-rect.y

    if (x >= 0 && x <= 480 && y >= 0 && y <= 480 && options.simulating == false && options.place.placing == true) {
        rbmk.columns[Math.floor(x/32) + rbmk.width * Math.floor(y/32)] = getConstructor(options.place.selected)
        if (options.place.selected != "nothing") {
            rbmk.columns[Math.floor(x/32) + rbmk.width * Math.floor(y/32)].x = Math.floor(x/32)
            rbmk.columns[Math.floor(x/32) + rbmk.width * Math.floor(y/32)].y = Math.floor(y/32)
        }
    }

    if (x >= 0 && x <= 480 && y >= 0 && y <= 480 && options.simulating == false && options.place.placing == false) {
        options.config.columnIndexSelected = Math.floor(x/32) + rbmk.width * Math.floor(y/32)
    }
    if (x >= 0 && x <= 480 && y >= 0 && y <= 480 && options.simulating == true) {
        options.config.columnIndexSelected = Math.floor(x/32) + rbmk.width * Math.floor(y/32)
    }

    var x = 0
    var y = 0
    options.place.blocks.forEach(block => {
        var rect = toolsRenderer.canvas.canvas.getBoundingClientRect()
        if (mPos[0]-rect.x > (x*32) && mPos[1]-rect.y > (y*32) && mPos[0]-rect.x < (x*32)+32 && mPos[1]-rect.y < (y*32)+32 && options.simulating == false && options.place.placing == true) {
            options.place.selected = block.select
        }
        x++
        if (x >= 14) {
            x = 0
            y++
        }
    })
})

// This just puts a list of rods
function fuelClick() {
    var btn = document.elementFromPoint(mPos[0], mPos[1])
    var col = rbmk.columns[options.config.columnIndexSelected]
    if (col instanceof Fuel) {
        col.fuel = new fuels[btn.getAttribute("fuelname")]
        options.config.prevColumn = -1
    }
}
var index = 0
fuels.forEach(fuel => {
    var test = new fuel()
    var element = document.createElement("img")
    element.src = test.texture
    element.style.width = "32px"
    element.style.height = "32px"

    var selfigniting = ""
    if (test.selfRate > 0 || test.function == EnumBurnFunc.SIGMOID) {
        selfigniting = `<p style="color: red; margin: 0px;">Self-igniting</p>`
    }

    element.setAttribute("onclick", "fuelClick()")
    element.setAttribute("fuelname", index)
    element.setAttribute("tooltip", `<b>${test.fullName}</b><br>${selfigniting}<p style="color: blue; margin: 0px;">Splits with: ${test.nType}</p><p style="color: blue; margin: 0px;">Splits into: ${test.rType}</p><p style="color: yellow; margin: 0px;">Flux function: <span style="color: white;">${test.getFuncDescription()}</span></p><p style="color: yellow; margin: 0px;">Function type: ${test.displayFunc}</p><p style="color: yellow; margin: 0px;">Xenon gen function: <span style="color: white;">x * ${test.xGen}</span></p><p style="color: yellow; margin: 0px;">Xenon burn function: <span style="color: white;">x² * ${test.xBurn}</span></p><p style="color: gold; margin: 0px;">Heat per flux: ${test.heat}°C</p><p style="color: gold; margin: 0px;">Diffusion: ${test.diffusion}¹/²</p><p style="color: red; margin: 0px;">Melting point: ${test.meltingPoint}°C</p>`)
    document.getElementById("fuels").appendChild(element)
    index++
})

rbmk.columns[112] = new Blank() // Marking the center
