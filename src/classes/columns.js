/*
    Sources from bits of code used:
        "Column" class functions: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKBase.java
        "Fuel" class functions: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKRod.java
        "Boiler" class update code: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKBoiler.java
        "Boiler" class turbine code: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/TileEntityMachineLargeTurbine.java
        "Boiler" class condensing code: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/TileEntityCondenser.java
        "Cooler" class update code: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKCooler.java
*/

// Column class
class Column
{
    constructor(index, tp) {
        this.index = index
        this.display = null

        this.moderated = false
        this.heat = 20
        this.maxHeat = 1500
        this.heatCache = []

        // ReaSim
        this.rs_steam = 0
        this.rs_water = 0
        this.rs_maxSteam = 16000
        this.rs_maxWater = 16000

        // Tooltip
        this.tooltip = tp.enabled
        this.tooltipText = tp.text

        // Other
        this.x = 0
        this.y = 0

        this.dirs = [
            {offsetX: 0, offsetY: -1},
            {offsetX: -1, offsetY: 0},
            {offsetX: 0, offsetY: 1},
            {offsetX: 1, offsetY: 0},
        ]
    }

    update(ticks, rbmk) {}

    update2(ticks, rbmk) {
        this.moveHeat(rbmk)
        if (RBMKDials.dialReasimBoilers == true) {
            this.boilWater()
        }
        this.coolPassively()

        if (this.rs_water > this.rs_maxWater)
            this.rs_water = this.rs_maxWater
    }

    boilWater() {
        if (this.heat < 100)
            return

        var heatConsumption = RBMKDials.dialBoilerHeatConsumption
        var availableHeat = (this.heat - 100) / heatConsumption
        var availableWater = this.rs_water
        var availableSpace = this.rs_maxSteam - this.rs_steam

        var processedWater = Math.floor(Math.min(availableHeat, Math.min(availableWater, availableSpace)) * RBMKDials.dialReasimBoilerSpeed)

        this.rs_water -= processedWater
        this.rs_steam += processedWater
        this.heat -= processedWater * heatConsumption
    }

    moveHeat(rbmk) {
        var rec = []
        rec.push(this)

        var heatTot = this.heat
        var waterTot = this.water
        var steamTot = this.steam

        var index = 0
        this.dirs.forEach(dir => {
            if (this.heatCache[index] != null)
                this.heatCache[index] = null

            if (this.heatCache[index] == null) {
                var column = rbmk.columns[(this.x - dir.offsetX) + rbmk.width * (this.y + dir.offsetY)]
                if (column instanceof Column) { // What is this check for anyway
                    this.heatCache[index] = column
                }
            }

            index++
        })

        this.heatCache.forEach(base => {
            if (base != null) {
                rec.push(base)
                heatTot += base.heat
                waterTot += base.water
                steamTot += base.steam
            }
        })

        var members = rec.length
        var stepSize = RBMKDials.dialColumnHeatFlow

        if (members > 1) {
            var targetHeat = heatTot / members

            var tWater = waterTot / members
            var rWater = waterTot % members
            var tSteam = steamTot / members
            var rSteam = steamTot % members

            rec.forEach(base => {
                var delta = targetHeat - base.heat
                base.heat += delta * stepSize

                base.rs_water = tWater
                base.rs_steam = tSteam
            })

            this.rs_water += rWater
            this.rs_steam += rSteam
        }
    }

    coolPassively() {
        this.heat -= RBMKDials.dialPassiveCooling

        if (this.heat < 20)
            this.heat = 20
    }

    // other funcs
    draw(ticks) {}
    reset() {}
    getConfig() {
        return `<p class="noMargin">Not configurable</p>`
    }
}

// Columns
class Blank extends Column 
{
    constructor() {
        super(0, {enabled: true, text: "<b>BLANK</b>"})
        this.display = new Display(15, this)
    }

    draw(ticks) {
        this.tooltipText = `<b>BLANK</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }
}
class Fuel extends Column 
{
    constructor() {
        super(1, {enabled: true, text: "<b>FUEL</b>"})
        this.display = new Display(1, this)
        this.display.draw2 = function(x, y, rbmk) {
            // Skin heat
            var h = quickMath(this.column.skinHeat - 20, 10, this.column.maxSkinHeat)
            var h2 = quickMath(this.column.skinHeat - 20, 26, this.column.maxSkinHeat)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+3,
                y: y*32+3+26-h2,
                w: 7,
                h: h2,
    
                sX: 11,
                sY: 183+(8-h),
                sW: 2,
                sH: h
            })
            // Depletion
            var h = quickMath(100-this.column.depletion, 8, 100)
            var h2 = quickMath(100-this.column.depletion, 26, 100)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+13,
                y: y*32+3+26-h2,
                w: 6,
                h: h2,
    
                sX: 14,
                sY: 183+(8-h),
                sW: 2,
                sH: h
            })
            // Xenon Poisoning
            var h = quickMath(this.column.xenonPoison, 8, 100)
            var h2 = quickMath(this.column.xenonPoison, 26, 100)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+22,
                y: y*32+3+26-h2,
                w: 7,
                h: h2,
    
                sX: 17,
                sY: 183+(8-h),
                sW: 2,
                sH: h
            })
        }

        this.stream = NType.ANY
        this.fluxFast = 0
        this.fluxSlow = 0

        this.coreHeat = 20
        this.skinHeat = 20
        this.maxSkinHeat = 20
        this.depletion = 100
        this.xenonPoison = 0

        this.fuel = new NONE()
        this.fuel.column = this

        this.moderated = false
    }

    // Flux stuff
    receiveFlux(type, flux) {
        switch (type) {
            case "FAST": this.fluxFast += flux; break
            case "SLOW": this.fluxSlow += flux; break
        }
    }
    fluxFromType(type) {
        switch (type) {
            case "SLOW": return this.fluxFast * 0.5 + this.fluxSlow
            case "FAST": return this.fluxFast + this.fluxSlow * 0.3
            case "ANY": return this.fluxFast + this.fluxSlow
        }

        return 0.0
    }
    spreadFlux(type, fluxOut, rbmk) {
        var range = RBMKDials.dialFluxRange

        this.dirs.forEach(dir => {
            this.stream = type
            var flux = fluxOut
                
            for (let i = 1; i <= range; i++) {
                flux = this.runInteraction(rbmk.columns[(this.x - dir.offsetX * i) + rbmk.width * (this.y + dir.offsetY * i)], flux)

                if (flux <= 0)
                    break
            }
        })
    }
    runInteraction(column, flux) {
        if (column instanceof Column) {
            if (column.moderated == true) {
                this.stream = NType.SLOW
            }
        }

        if (column instanceof Fuel) {
            if (column.fuel != null) {
                column.receiveFlux(this.stream, flux)
                return 0
            } else {
                return flux
            }
        }

        if (column instanceof Outgasser) {
            // if rod can process
            // return flux
        }

        /*if(te instanceof IRBMKFluxReceiver) {
			IRBMKFluxReceiver rod = (IRBMKFluxReceiver)te;
			rod.receiveFlux(stream, flux);
			return 0;
		}*/

        if (column instanceof Control) {
            var mult = column.level
            if (mult == 0)
                return 0

            flux *= mult

            return flux
        }

        if (column instanceof Moderator) {
            this.stream = NType.SLOW
            return flux
        }

        if (column instanceof Reflector) {
            this.receiveFlux(this.moderated ? NType.SLOW : this.stream, flux)
            return 0
        }

        if (column instanceof Absorber) {
            return 0
        }

        if (column instanceof Column) {
            return flux
        }

        return 0
    }

    // Other
    update2(ticks) {}
    update(ticks, rbmk) {
        // Fuel stuff
        if (this.fuel.constructor.name == "NONE") {
            this.depletion = 100
            this.coreHeat = 20
            this.skinHeat = 20
            this.maxSkinHeat = 20

            this.fluxFast = 0
            this.fluxSlow = 0
        } if (this.fuel.constructor.name != "NONE") {
            this.depletion = this.fuel.calcDepletion()
            this.xenonPoison = this.fuel.xenon
            this.coreHeat = this.fuel.coreHeat
            this.skinHeat = this.fuel.skinHeat
            
            // ACTUAL flux stuff
            var fluxIn = this.fluxFromType(this.fuel.nType)
            var fluxOut = this.fuel.burn(fluxIn)
            var rType = this.fuel.rType

            this.fuel.updateHeat(1)
            this.heat += this.fuel.provideHeat(this.heat, 1)

            if (this.heat > this.maxHeat && RBMKDials.dialDisableMeltdowns == false) {
                rbmk.meltdown()
                return
            }

            this.fluxFast = 0
            this.fluxSlow = 0

            this.spreadFlux(rType, fluxOut, rbmk)
        }
    }

    draw(ticks) {
        var moderated = ""
        if (this.moderated == true) {
            moderated = `<p style="color: yellow; margin: 0px;">Moderated</p>`
        }
        this.tooltipText = `<b>FUEL</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p><p style="color: green; margin: 0px;">Depletion: ${this.depletion.toFixed(3)}%</p><p style="color: purple; margin: 0px;">Xenon poison: ${this.xenonPoison.toFixed(1)}%</p><p style="color: red; margin: 0px;">Core temperature: ${this.coreHeat.toFixed(1)}°C</p><p style="color: rgb(255, 63, 63); margin: 0px;">Skin temperature: ${this.skinHeat.toFixed(1)}°C / ${this.maxSkinHeat.toFixed(1)}°C</p>${moderated}`
    }

    reset() {
        this.coreHeat = 20
        this.skinHeat = 20
        if (this.fuel.constructor.name == "NONE") {
            this.depletion = 100
            this.maxSkinHeat = 20
        } if (this.fuel.constructor.name != "NONE") {
            this.depletion = 0
            this.maxSkinHeat = this.fuel.meltingPoint
        }

        this.fuel.yieldd = this.fuel.defaultYield
        this.fuel.coreHeat = 20
        this.fuel.skinHeat = 20
        
        this.fluxFast = 0
        this.fluxSlow = 0

        this.xenonPoison = 0
    }

    getConfig() {
        var selfigniting = ""
        if (this.fuel.selfRate > 0 || this.fuel.function == EnumBurnFunc.SIGMOID) {
            selfigniting = `<p style="color: red; margin: 0px;">Self-igniting</p>`
        }

        var stuff = document.createElement("div")
    
        var fuelTooltip = `<b>${this.fuel.fullName}</b><br>${selfigniting}<p style="color: green; margin: 0px;">Depletion: ${this.fuel.calcDepletion().toFixed(3)}%</p><p style="color: purple; margin: 0px;">Xenon poison: ${(this.xenonPoison * 1000) / 1000}%</p><p style="color: blue; margin: 0px;">Splits with: ${this.fuel.nType}</p><p style="color: blue; margin: 0px;">Splits into: ${this.fuel.rType}</p><p style="color: yellow; margin: 0px;">Flux function: <span style="color: white;">${this.fuel.getFuncDescription()}</span></p><p style="color: yellow; margin: 0px;">Function type: ${this.fuel.displayFunc}</p><p style="color: yellow; margin: 0px;">Xenon gen function: <span style="color: white;">x * ${this.fuel.xGen}</span></p><p style="color: yellow; margin: 0px;">Xenon burn function: <span style="color: white;">x² * ${this.fuel.xBurn}</span></p><p style="color: gold; margin: 0px;">Heat per flux: ${this.fuel.heat}°C</p><p style="color: gold; margin: 0px;">Diffusion: ${this.fuel.diffusion}¹/²</p><p style="color: rgb(255, 63, 63); margin: 0px;">Skin temp: ${this.fuel.skinHeat.toFixed(1)}°C</p><p style="color: rgb(255, 63, 63); margin: 0px;">Core temp: ${this.fuel.coreHeat.toFixed(1)}°C</p><p style="color: red; margin: 0px;">Melting point: ${this.fuel.meltingPoint.toFixed(1)}°C</p>`
        var element = document.createElement("p")
        element.className = "noMargin"
        element.style.fontSize = "27px"
        element.innerHTML = "Fuel: "
        var element2 = document.createElement("img")
        element2.src = this.fuel.texture
        element2.setAttribute("tooltip", fuelTooltip)
        element.insertAdjacentElement("beforeend", element2)
        stuff.insertAdjacentElement("beforeend", element)

        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Moderated: ${this.moderated}`
        elementt.setAttribute("configmenuaction", "moderation")
        elementt.setAttribute("onclick", `configMenuAction()`)
        stuff.insertAdjacentElement("beforeend", elementt)

        return stuff
    }
}
class Control extends Column 
{
    constructor() {
        super(2, {enabled: true, text: "<b>CONTROL</b>"})
        this.display = new Display(2, this)
        this.display.draw2 = function(x, y, rbmk) {
            // Rod Level
            var h = quickMath(100-(this.column.level*100), 8, 100)
            var h2 = quickMath(100-(this.column.level*100), 26, 100)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+13,
                y: y*32+3,
                w: 6,
                h: h2,
    
                sX: 24,
                sY: 183,
                sW: 2,
                sH: h
            })
        }

        this.level = 0
        this.lastLevel = 0
        this.speed = 0.00277
        this.targetLevel = 0
    }

    update(ticks) {
        this.lastLevel = this.level

        if (this.level < this.targetLevel) {
            this.level += this.speed * RBMKDials.dialControlSpeed

            if (this.level > this.targetLevel)
                this.level = this.targetLevel
        }

        if (this.level > this.targetLevel) {
            this.level -= this.speed * RBMKDials.dialControlSpeed

            if (this.level < this.targetLevel)
                this.level = this.targetLevel
        }
    }

    draw(ticks) {
        var moderated = ""
        if (this.moderated == true) {
            moderated = `<p style="color: yellow; margin: 0px;">Moderated</p>`
        }
        this.tooltipText = `<b>CONTROL</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p><p style="color: yellow; margin: 0px;">${Math.floor(this.level*100)}%</p>${moderated}`
    }

    reset() {
        this.level = 0
        this.targetLevel = 0
        this.lastLevel = 0
    }

    getConfig() {
        var stuff = document.createElement("div")

        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Insert control rod`
        elementt.setAttribute("configmenuaction", "pull")
        elementt.setAttribute("onclick", `configMenuAction()`)
        stuff.insertAdjacentElement("beforeend", elementt)

        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Moderated: ${this.moderated}`
        elementt.setAttribute("configmenuaction", "moderation")
        elementt.setAttribute("onclick", `configMenuAction()`)
        stuff.insertAdjacentElement("beforeend", elementt)

        return stuff
    }
}
class ControlAuto extends Column 
{
    constructor() {
        super(3, {enabled: true, text: "<b>CONTROL_AUTO</b>"})
        this.display = new Display(3, this)
        this.display.draw2 = function(x, y, rbmk) {
            // Rod Level
            var h = quickMath(100-this.column.level, 8, 100)
            var h2 = quickMath(100-this.column.level, 26, 100)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+13,
                y: y*32+3,
                w: 6,
                h: h2,
    
                sX: 34,
                sY: 183,
                sW: 2,
                sH: h
            })
        }

        this.level = 0
        this.function = 0

        this.heatLower = 0
        this.heatUpper = 0
        this.levelLower = 0
        this.levelUpper = 0
    }

    draw(ticks) {
        this.tooltipText = `<b>CONTROL_AUTO</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p><p style="color: yellow; margin: 0px;">${this.level}%</p>`
    }

    reset() {
        this.level = 0
    }
}
class Boiler extends Column 
{
    constructor() {
        super(4, {enabled: true, text: "<b>BOILER</b>"})
        this.display = new Display(4, this)
        this.display.draw2 = function(x, y, rbmk) {
            // Feedwater
            var h = quickMath(this.column.feedwater, 8, this.column.feedwaterMax)
            var h2 = quickMath(this.column.feedwater, 26, this.column.feedwaterMax)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+3,
                y: y*32+3+26-h2,
                w: 10,
                h: h2,
    
                sX: 41,
                sY: 183+(8-h),
                sW: 3,
                sH: h
            })
            // Compressor
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+13,
                y: y*32+3+(6.6*this.column.steamType),
                w: 6,
                h: 6,
    
                sX: 44,
                sY: 183+(2*this.column.steamType),
                sW: 2,
                sH: 2
            })
            // Steam
            var h = quickMath(this.column.steam, 8, this.column.steamMax)
            var h2 = quickMath(this.column.steam, 26, this.column.steamMax)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+19,
                y: y*32+3+26-h2,
                w: 10,
                h: h2,
    
                sX: 46,
                sY: 183+(8-h),
                sW: 3,
                sH: h
            })
        }

        this.feedwater = 0
        this.feedwaterMax = 10000

        this.steam = 0
        this.steamMax = 1000000

        this.steamType = 0
        this.compressors = ["Steam", "Dense Steam", "Super Dense Steam", "Ultra Dense Steam"]

        // These other variables are for boiler output rate
        this.steam2 = 0
        this.steamMax2 = 512000

        this.spentSteam = 0
        this.spentSteamMax = 10240000

        this.producedPower = 0
    }

    update(ticks) {
        if (this.feedwater > this.feedwaterMax) 
            this.feedwater = this.feedwaterMax

        if (this.spentSteam > this.spentSteamMax) 
            this.spentSteam = this.spentSteamMax

        // Boiling
        var heatCap = this.getHeatFromSteam()
        var heatProvided = this.heat - heatCap

        if (heatProvided > 0) {
            var HEAT_PER_MB_WATER = RBMKDials.dialBoilerHeatConsumption
            var steamFactor = this.getFactorFromSteam()
            var waterUsed = 0
            var steamProduced = 0

            if (this.steamType == 3) {
                steamProduced = Math.floor((heatProvided / HEAT_PER_MB_WATER) * 100 / steamFactor)
                waterUsed = Math.floor(steamProduced / 100 * steamFactor)

                if (this.feedwater < waterUsed) {
                    steamProduced = Math.floor(this.feedwater * 100 / steamFactor)
                    waterUsed = Math.floor(steamProduced / 100 * steamFactor)
                }
            } else {
                waterUsed = Math.floor(heatProvided / HEAT_PER_MB_WATER)
                waterUsed = Math.min(waterUsed, this.feedwater)
                steamProduced = Math.floor((waterUsed * 100) / steamFactor)
            }

            this.feedwater -= waterUsed
            this.steam += steamProduced

            if (this.steam > this.steamMax) 
                this.steam = this.steamMax

            this.heat -= waterUsed * HEAT_PER_MB_WATER
        }

        // Steal steam from the boiler and put them through processing
        this.steam -= options.rbmkStuff.boilerOutputRate
        this.steam2 += options.rbmkStuff.boilerOutputRate

        if (this.steam2 > this.steamMax2) 
            this.steam2 = this.steamMax2
        if (this.steam < 0) 
            this.steam = 0

        // Turbine (large turbine)
        this.producedPower = 0
        if (options.rbmkStuff.boilerOutputRate > 0) {
            var trait = this.getTraitFromSteam()
            var eff = trait.efficiency

            if (eff > 0) {
                var inputOps = Math.floor(this.steam2 / trait.amountReq)
                var outputOps = (this.spentSteamMax - this.spentSteam)
                var cap = Math.ceil(this.steam2 / trait.amountReq / 5)
                var ops = Math.min(inputOps, Math.min(outputOps, cap))
    
                this.steam2 -= ops * trait.amountReq
                this.spentSteam += ops * trait.amountProduced
    
                this.producedPower += ops * trait.heatEnergy * eff
            }
        }

        // Condensation
        var convert = Math.min(this.spentSteam, this.feedwaterMax - this.feedwater)
        this.spentSteam -= convert
        this.feedwater += convert
    }
    // Other steam stuff
    getHeatFromSteam() {
        if (this.steamType == 0) return 100
        if (this.steamType == 1) return 300
        if (this.steamType == 2) return 450
        if (this.steamType == 3) return 600
    }
    getFactorFromSteam() {
        if (this.steamType == 0) return 1
        if (this.steamType == 1) return 10
        if (this.steamType == 2) return 100
        if (this.steamType == 3) return 1000
    }
    getTraitFromSteam() {
        if (this.steamType == 0) return {amountReq: 100, amountProduced: 1, efficiency: 1, heatEnergy: 200}
        if (this.steamType == 1) return {amountReq: 1, amountProduced: 10, efficiency: 1, heatEnergy: 2}
        if (this.steamType == 2) return {amountReq: 1, amountProduced: 10, efficiency: 1, heatEnergy: 18}
        if (this.steamType == 3) return {amountReq: 1, amountProduced: 10, efficiency: 1, heatEnergy: 120}
    }

    // Other
    draw(ticks) {
        this.tooltipText = `<b>BOILER</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p><p style="color: blue; margin: 0px;">Feedwater: ${this.feedwater} / ${this.feedwaterMax}</p><p style="color: white; margin: 0px;">Steam: ${this.steam} / ${this.steamMax}</p><p style="color: yellow; margin: 0px;">Compressor: ${this.compressors[this.steamType]}</p>`
    }

    reset() {
        this.feedwater = 0
        this.steam = 0
        this.steam2 = 0
        this.spentSteam = 0
        this.producedPower = 0
    }

    getConfig() {
        var stuff = document.createElement("div")
        
        var elementt = document.createElement("button")
        elementt.className = "textButton"
        elementt.style.fontSize = "27px"
        elementt.innerHTML = `Steam compression: ${this.compressors[this.steamType]}`
        elementt.setAttribute("configmenuaction", "compression")
        elementt.setAttribute("onclick", `configMenuAction()`)
        stuff.insertAdjacentElement("beforeend", elementt)

        return stuff
    }
}
class Moderator extends Column 
{
    constructor() {
        super(5, {enabled: true, text: "<b>MODERATOR</b>"})
        this.display = new Display(5, this)
    }

    draw(ticks) {
        this.tooltipText = `<b>MODERATOR</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }
}
class Absorber extends Column 
{
    constructor() {
        super(6, {enabled: true, text: "<b>ABSORBER</b>"})
        this.display = new Display(6, this)
    }

    draw(ticks) {
        this.tooltipText = `<b>ABSORBER</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }
}
class Reflector extends Column 
{
    constructor() {
        super(7, {enabled: true, text: "<b>REFLECTOR</b>"})
        this.display = new Display(7, this)
    }

    draw(ticks) {
        this.tooltipText = `<b>REFLECTOR</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }
}
class Outgasser extends Column 
{
    constructor() {
        super(8, {enabled: true, text: "<b>OUTGASSER</b>"})
        this.display = new Display(8, this)

        this.gas = 0
        this.gasMax = 0
        this.gasType = 9
        this.progress = 0
    }

    draw(ticks) {
        this.tooltipText = `<b>OUTGASSER</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }

    reset() {
        this.gas = 0
        this.progress = 0
    }
}
// 9th is fuel sim
// idk about the 10th, might be unused
class Storage extends Column 
{
    constructor() {
        super(11, {enabled: true, text: "<b>STORAGE</b>"})
        this.display = new Display(11, this)
    }

    draw(ticks) {
        this.tooltipText = `<b>STORAGE</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }
}
class Cooler extends Column 
{
    constructor() {
        super(12, {enabled: true, text: "<b>COOLER</b>"})
        this.display = new Display(12, this)

        this.cooled = 0
        this.cryo = 0
        this.cryoMax = 8000
        this.cryoType = 37
    }

    update(ticks) {
        if (this.cryo > this.cryoMax) 
            this.cryo = this.cryoMax

        if (this.heat > 750) {
            var heatProvided = this.heat - 750
            var cooling = Math.min(heatProvided, this.cryo)

            this.heat -= cooling
            this.cryo -= cooling
        }
    }

    draw(ticks) {
        this.tooltipText = `<b>COOLER</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p>`
    }

    reset() {
        this.cooled = 0
        this.cryo = 0
    }
}
class Heatex extends Column 
{
    constructor() {
        super(13, {enabled: true, text: "<b>HEATEX</b>"})
        this.display = new Display(13, this)
        this.display.draw2 = function(x, y, rbmk) {
            // Coolant
            var h = quickMath(this.column.coolant, 8, this.column.coolantMax)
            var h2 = quickMath(this.column.coolant, 26, this.column.coolantMax)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+3,
                y: y*32+3+26-h2,
                w: 10,
                h: h2,
    
                sX: 131,
                sY: 184+(8-h),
                sW: 3,
                sH: h
            })
            // Hot Coolant
            var h = quickMath(this.column.hotcoolant, 8, this.column.hotcoolantMax)
            var h2 = quickMath(this.column.hotcoolant, 26, this.column.hotcoolantMax)
            rbmk.statsRenderer.draw("image", {
                img: rbmk.consoleImg,
                crop: true,
    
                x: x*32+19,
                y: y*32+3+26-h2,
                w: 10,
                h: h2,
    
                sX: 136,
                sY: 183+(8-h),
                sW: 3,
                sH: h
            })
        }

        this.coolant = 0
        this.coolantMax = 16000

        this.hotcoolant = 0
        this.hotcoolantMax = 16000
    }

    draw(ticks) {
        this.tooltipText = `<b>HEATEX</b><br><p style="color: yellow; margin: 0px;">Column temperature: ${this.heat.toFixed(1)}°C</p><p style="color: blue; margin: 0px;">Coolant: ${this.coolant} / ${this.coolantMax}</p><p style="color: red; margin: 0px;">Hot Coolant: ${this.hotcoolant} / ${this.hotcoolantMax}</p>`
    }

    reset() {
        this.coolant = 0
        this.hotcoolant = 0
    }
}