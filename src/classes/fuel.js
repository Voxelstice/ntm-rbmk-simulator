/*
    Sources from bits of code used:
        The "RBMKFuel" class: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/machine/ItemRBMKRod.java
        The existing fuel classes: https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/ModItems.java#L3572
*/

const NType = {
    SLOW: "SLOW",
    FAST: "FAST",
    ANY: "ANY"
}
const EnumBurnFunc = {
    PASSIVE: "PASSIVE",
    LOG_TEN: "LOG_TEN",
    PLATEU: "PLATEU",
    ARCH: "ARCH",
    SIGMOID: "SIGMOID",
    SQUARE_ROOT: "SQUARE_ROOT",
    LINEAR: "LINEAR",
    QUADRATIC: "QUADRATIC",
    EXPERIMENTAL: "EXPERIMENTAL"
}
const EnumBurnFunc2 = {
    PASSIVE: `<span style="color: green; margin: 0px;">SAFE / PASSIVE</span>`,
    LOG_TEN: `<span style="color: yellow; margin: 0px;">MEDIUM / LOGARITHMIC</span>`,
    PLATEU: `<span style="color: green; margin: 0px;">SAFE / EULER</span>`,
    ARCH: `<span style="color: red; margin: 0px;">DANGEROUS / NEGATIVE-QUADRATIC</span>`,
    SIGMOID: `<span style="color: green; margin: 0px;">SAFE / SIGMOID</span>`,
    SQUARE_ROOT: `<span style="color: yellow; margin: 0px;">MEDIUM / SQUARE ROOT</span>`,
    LINEAR: `<span style="color: red; margin: 0px;">DANGEROUS / LINEAR</span>`,
    QUADRATIC: `<span style="color: red; margin: 0px;">DANGEROUS / QUADRATIC</span>`,
    EXPERIMENTAL: `<span style="color: red; margin: 0px;">EXPERIMENTAL / SINE SLOPE</span>`
}
const EnumDepleteFunc = {
    LINEAR: "LINEAR",
    RAISING_SLOPE: "RAISING_SLOPE",
    BOOSTED_SLOPE: "BOOSTED_SLOPE",
    GENTLE_SLOPE: "GENTLE_SLOPE",
    STATIC: "STATIC"
}

class RBMKFuel {
    constructor() {
        this.column = null

        this.fullName = ""
        this.reactivity = ""
        this.selfRate = 0
        this.function = EnumBurnFunc.LOG_TEN
        this.displayFunc = EnumBurnFunc2.LOG_TEN
        this.depFunc = EnumDepleteFunc.GENTLE_SLOPE
        this.xGen = 0.5
        this.xBurn = 50
        this.heat = 1
        // This is yieldd because of Uncaught SyntaxError: yield is a reserved identifier
        this.yieldd = 0
        this.defaultYield = 0
        this.meltingPoint = 1000
        this.diffusion = 0.2
        this.nType = NType.SLOW
        this.rType = NType.FAST

        this.xenon = 0
        this.coreHeat = 20
        this.skinHeat = 20
        
        this.texture = "https://raw.githubusercontent.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/master/src/main/resources/assets/hbm/textures/items/rbmk_fuel_empty.png"
    }

    // Used when constructing fuels
    setYield(yieldd) {
        this.yieldd = yieldd
        this.defaultYield = yieldd
    }
    setStats(funcEnd, selfRate) {
        this.reactivity = funcEnd
        this.selfRate = selfRate || 0
    }
    setFunction(func) {
        this.function = func
        this.displayFunc = EnumBurnFunc2[func]
    }
    setDepletionFunction(func) {
        this.depFunc = func
    }
    setXenon(xGen, xBurn) {
        this.xGen = xGen
        this.xBurn = xBurn
    }
    setDiffusion(diffusion) {
        this.diffusion = diffusion
    }
    setHeat(heat) {
        this.heat = heat
    }
    setMeltingPoint(meltingPoint) {
        this.meltingPoint = meltingPoint
    }
    setName(name) {
        this.fullName = name
    }
    setTexture(tex) {
        // I have decided to make it just load textures from the GitHub repo instead of shoving every single texture inside the site files
        this.texture = `https://raw.githubusercontent.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/master/src/main/resources/assets/hbm/textures/items/${tex}.png`
    }
    setNeutronTypes(nType, rType) {
        this.nType = nType
        this.rType = rType || NType.FAST
    }

    // Tooltip
    calcDepletion() {
        return (((this.defaultYield - this.yieldd) / this.defaultYield) * 100000) / 1000
    }
    getFuncDescription() {
        var func = ""

        switch (this.function) {
            case "PASSIVE": 
                func = `${this.selfRate}`
			    break
		    case "LOG_TEN": 
                func = "log10(%1$s + 1) * 0.5 * %2$s"
			    break
		    case "PLATEU": 
                func = "(1 - e^-%1$s / 25)) * %2$s"
		        break
		    case "ARCH": 
                func = "(%1$s - %1$s² / 10000) / 100 * %2$s [0;∞]"
		    	break
		    case "SIGMOID": 
                func = "%2$s / (1 + e^(-(%1$s - 50) / 10)"
		    	break
		    case "SQUARE_ROOT": 
                func = "sqrt(%1$s) * %2$s / 10"
		    	break
		    case "LINEAR": 
                func = "%1$s / 100 * %2$s"
		    	break
		    case "QUADRATIC": 
                func = "%1$s² / 10000 * %2$s"
		    	break
		    case "EXPERIMENTAL": 
                func = "%1$s * (sin(%1$s) + 1) * %2$s"
			    break
		    default: 
                func = "ERROR"
                break
        }

        var enrichment = this.getEnrichment()
        if (enrichment < 1) {
            enrichment = this.reactivityModByEnrichment(enrichment)
            var reactivity = `<span style="color: yellow;">${(this.reactivity * enrichment * 1000) / 1000}</span>`
            var enrichmentPer = `<span style="color: gold;">${(enrichment * 1000) / 10}%</span>`

            return func.replaceAll("%1$s", this.selfRate > 0 ? `(x <span style="color: red;">+ ${this.selfRate}</span>)` : "x").replaceAll("%2$s", reactivity).concat(enrichmentPer)
        }

        
        return func.replaceAll("%1$s", this.selfRate > 0 ? `(x <span style="color: red;">+ ${this.selfRate}</span>)` : "x").replaceAll("%2$s", this.reactivity)
    }

    // Actual functions used in simulation
    burn(inFlux) {
        inFlux += this.selfRate

        var xenon = this.xenon
        xenon -= this.xenonBurnFunc(inFlux)

        inFlux *= (1 - this.getPoisonLevel())

        xenon += this.xenonGenFunc(inFlux)

        if(xenon < 0) xenon = 0
        if(xenon > 100) xenon = 100

        this.xenon = xenon

        var outFlux = this.reactivityFunc(inFlux, this.getEnrichment() * RBMKDials.dialReactivityMod)
        var y = this.yieldd
        y -= inFlux

        if(y < 0) y = 0

        this.yieldd = y

        var coreHeat = this.coreHeat
        coreHeat += outFlux * this.heat

        this.coreHeat = this.rectify(coreHeat)

        return outFlux
    }
    updateHeat(mod) {
        var coreHeat = this.coreHeat
        var hullHeat = this.skinHeat

        if (coreHeat > hullHeat) {
            var mid = (coreHeat = hullHeat) / 2

            coreHeat -= mid * this.diffusion * RBMKDials.dialDiffusionMod * mod
            hullHeat += mid * this.diffusion * RBMKDials.dialDiffusionMod * mod

            this.coreHeat = this.rectify(coreHeat)
            this.skinHeat = this.rectify(hullHeat)
        }
    }
    provideHeat(heat, mod) {
        var hullHeat = this.skinHeat

        if (hullHeat > this.meltingPoint) {
            var coreHeat = this.coreHeat
            var avg = (heat + hullHeat + coreHeat) / 3
            this.coreHeat = avg
            this.hullHeat = avg
            return avg - heat
        }

        if (hullHeat <= heat)
            return 0

        var ret = (hullHeat - heat) / 2
        ret *= RBMKDials.dialHeatProvision * mod

        hullHeat -= ret
        this.skinHeat = hullHeat

        return ret
    }

    // Equations
    reactivityFunc(inFlux, enrichment) {
        var flux = inFlux * this.reactivityModByEnrichment(enrichment)

        switch (this.function) {
            case "PASSIVE": return this.selfRate * enrichment;
		    case "LOG_TEN": return Math.log10(flux + 1) * 0.5 * this.reactivity;
		    case "PLATEU": return (1 - Math.pow(Math.E, -flux / 25)) * this.reactivity;
		    case "ARCH": return Math.max((flux - (flux * flux / 10000)) / 100 * this.reactivity, 0);
		    case "SIGMOID": return this.reactivity / (1 + Math.pow(Math.E, -(flux - 50) / 10));
		    case "SQUARE_ROOT": return Math.sqrt(flux) * this.reactivity / 10;
		    case "LINEAR": return flux / 100 * this.reactivity;
		    case "QUADRATIC": return flux * flux / 10000 * this.reactivity;
		    case "EXPERIMENTAL": return flux * (Math.sin(flux) + 1) * this.reactivity;
        }

        return 0
    }
    reactivityModByEnrichment(enrichment) {
        switch (this.depFunc) {
            default:
            case "LINEAR": return enrichment
            case "STATIC": return 1
            case "BOOSTED_SLOPE": return enrichment + Math.sin((enrichment - 1) * (enrichment - 1) * Math.PI)
            case "RAISING_SLOPE": return enrichment + (Math.sin(enrichment * Math.PI) / 2)
            case "GENTLE_SLOPE": return enrichment + (Math.sin(enrichment * Math.PI) / 3)
        }
    }

    xenonGenFunc(flux) {
        return flux * this.xGen
    }
    xenonBurnFunc(flux) {
        return (flux * flux) / this.xBurn
    }

    // Other
    getPoisonLevel() {
        return this.xenon / 100
    }
    getEnrichment() {
        return this.yieldd / this.defaultYield
    }
    rectify(num) {
		if(num > 1_000_000) num = 1_000_000
		if(num < 20 || isNaN(num)) num = 20
		
		return num;
	}
}

// Fuels
// TODO: Automate this
class NONE extends RBMKFuel {
    constructor() {
        super()

        this.setName("rbmk_fuel_empty")
    }
}
class UEU extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(15)
        this.setFunction(EnumBurnFunc.LOG_TEN)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setHeat(0.65)
        this.setMeltingPoint(2865)
        this.setName("rbmk_fuel_ueu")
        this.setTexture("rbmk_fuel_ueu")
    }
}
class MEU extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(20)
        this.setFunction(EnumBurnFunc.LOG_TEN)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setHeat(0.65)
        this.setMeltingPoint(2865)
        this.setName("rbmk_fuel_meu")
        this.setTexture("rbmk_fuel_meu")
    }
}
class HEU233 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(27.5)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setHeat(1.25)
        this.setMeltingPoint(2865)
        this.setName("rbmk_fuel_heu233")
        this.setTexture("rbmk_fuel_heu233")
    }
}
class HEU235 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(50)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setMeltingPoint(2865)
        this.setName("rbmk_fuel_heu235")
        this.setTexture("rbmk_fuel_heu235")
    }
}
class THMEU extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(20)
        this.setFunction(EnumBurnFunc.PLATEU)
        this.setDepletionFunction(EnumDepleteFunc.BOOSTED_SLOPE)
        this.setHeat(0.65)
        this.setMeltingPoint(3350)
        this.setName("rbmk_fuel_thmeu")
        this.setTexture("rbmk_fuel_thmeu")
    }
}
class LEP extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(35)
        this.setFunction(EnumBurnFunc.LOG_TEN)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setHeat(0.75)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_lep")
        this.setTexture("rbmk_fuel_lep")
    }
}
class MEP extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(35, 20)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_mep")
        this.setTexture("rbmk_fuel_mep")
    }
}
class HEP239 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(30)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setHeat(1.25)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_hep")
        this.setTexture("rbmk_fuel_hep")
    }
}
class HEP241 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(40)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setHeat(1.75)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_hep241")
        this.setTexture("rbmk_fuel_hep241")
    }
}
class LEA extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(60, 10)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setHeat(1.5)
        this.setMeltingPoint(2386)
        this.setName("rbmk_fuel_lea")
        this.setTexture("rbmk_fuel_lea")
    }
}
class MEA extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(35, 20)
        this.setFunction(EnumBurnFunc.ARCH)
        this.setHeat(1.75)
        this.setMeltingPoint(2386)
        this.setName("rbmk_fuel_mea")
        this.setTexture("rbmk_fuel_mea")
    }
}
class HEA241 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(65, 15)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setHeat(1.85)
        this.setMeltingPoint(2386)
        this.setNeutronTypes(NType.FAST, NType.FAST)
        this.setName("rbmk_fuel_hea241")
        this.setTexture("rbmk_fuel_hea241")
    }
}
class HEA242 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(45)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setHeat(2)
        this.setMeltingPoint(2386)
        this.setName("rbmk_fuel_hea242")
        this.setTexture("rbmk_fuel_hea242")
    }
}
class MEN extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(30)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setHeat(0.75)
        this.setMeltingPoint(2800)
        this.setNeutronTypes(NType.ANY, NType.FAST)
        this.setName("rbmk_fuel_men")
        this.setTexture("rbmk_fuel_men")
    }
}
class HEN extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(40)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setMeltingPoint(2800)
        this.setNeutronTypes(NType.FAST, NType.FAST)
        this.setName("rbmk_fuel_hen")
        this.setTexture("rbmk_fuel_hen")
    }
}
class MOX extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(40)
        this.setFunction(EnumBurnFunc.LOG_TEN)
        this.setDepletionFunction(EnumDepleteFunc.RAISING_SLOPE)
        this.setMeltingPoint(2815)
        this.setName("rbmk_fuel_mox")
        this.setTexture("rbmk_fuel_mox")
    }
}
class LES extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(50)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setHeat(1.25)
        this.setMeltingPoint(2800)
        this.setNeutronTypes(NType.SLOW, NType.SLOW)
        this.setName("rbmk_fuel_les")
        this.setTexture("rbmk_fuel_les")
    }
}
class MES extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(75)
        this.setFunction(EnumBurnFunc.ARCH)
        this.setHeat(1.5)
        this.setMeltingPoint(2750)
        this.setName("rbmk_fuel_mes")
        this.setTexture("rbmk_fuel_mes")
    }
}
class HES extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(90)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setHeat(1.75)
        this.setMeltingPoint(3000)
        this.setName("rbmk_fuel_hes")
        this.setTexture("rbmk_fuel_hes")
    }
}
class LEAUS extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(30)
        this.setFunction(EnumBurnFunc.SIGMOID)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.05, 50)
        this.setHeat(1.5)
        this.setMeltingPoint(7029)
        this.setName("rbmk_fuel_leaus")
        this.setTexture("rbmk_fuel_leaus")
    }
}
class HEAUS extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(35)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setXenon(0.05, 50)
        this.setHeat(2)
        this.setMeltingPoint(5211)
        this.setName("rbmk_fuel_heaus")
        this.setTexture("rbmk_fuel_heaus")
    }
}
class PO210BE extends RBMKFuel {
    constructor() {
        super()

        this.setYield(25000000)
        this.setStats(0, 50)
        this.setFunction(EnumBurnFunc.PASSIVE)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.0, 50)
        this.setHeat(0.1)
        this.setDiffusion(0.05)
        this.setMeltingPoint(1287)
        this.setNeutronTypes(NType.SLOW, NType.SLOW)
        this.setName("rbmk_fuel_po210be")
        this.setTexture("rbmk_fuel_po210be")
    }
}
class RA226BE extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(0, 20)
        this.setFunction(EnumBurnFunc.PASSIVE)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.0, 50)
        this.setHeat(0.035)
        this.setDiffusion(0.5)
        this.setMeltingPoint(700)
        this.setNeutronTypes(NType.SLOW, NType.SLOW)
        this.setName("rbmk_fuel_ra226be")
        this.setTexture("rbmk_fuel_ra226be")
    }
}
class PU238BE extends RBMKFuel {
    constructor() {
        super()

        this.setYield(50000000)
        this.setStats(40, 40)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setHeat(0.1)
        this.setDiffusion(0.05)
        this.setMeltingPoint(1287)
        this.setNeutronTypes(NType.SLOW, NType.SLOW)
        this.setName("rbmk_fuel_pu238be")
        this.setTexture("rbmk_fuel_pu238be")
    }
}
class BALEFIRE_GOLD extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(40, 40)
        this.setFunction(EnumBurnFunc.ARCH)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.0, 50)
        this.setMeltingPoint(2000)
        this.setName("rbmk_fuel_balefire_gold")
        this.setTexture("rbmk_fuel_balefire_gold")
    }
}
class FLASHLEAD extends RBMKFuel {
    constructor() {
        super()

        this.setYield(250000000)
        this.setStats(40, 50)
        this.setFunction(EnumBurnFunc.ARCH)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.0, 50)
        this.setMeltingPoint(2050)
        this.setName("rbmk_fuel_flashlead")
        this.setTexture("rbmk_fuel_flashlead")
    }
}
class BALEFIRE extends RBMKFuel {
    constructor() {
        super()

        this.setYield(100000000)
        this.setStats(100, 35)
        this.setFunction(EnumBurnFunc.ARCH)
        this.setDepletionFunction(EnumDepleteFunc.LINEAR)
        this.setXenon(0.0, 50)
        this.setHeat(3)
        this.setMeltingPoint(3652)
        this.setName("rbmk_fuel_balefire")
        this.setTexture("rbmk_fuel_balefire")
    }
}
class ZFB_BISMUTH extends RBMKFuel {
    constructor() {
        super()

        this.setYield(50000000)
        this.setStats(20)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setHeat(1.75)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_zfb_bismuth")
        this.setTexture("rbmk_fuel_zfb_bismuth")
    }
}
class ZFB_PU241 extends RBMKFuel {
    constructor() {
        super()

        this.setYield(50000000)
        this.setStats(20)
        this.setFunction(EnumBurnFunc.SQUARE_ROOT)
        this.setMeltingPoint(2865)
        this.setName("rbmk_fuel_zfb_pu241")
        this.setTexture("rbmk_fuel_zfb_pu241")
    }
}
class ZFB_AM_MIX extends RBMKFuel {
    constructor() {
        super()

        this.setYield(50000000)
        this.setStats(20)
        this.setFunction(EnumBurnFunc.LINEAR)
        this.setHeat(1.75)
        this.setMeltingPoint(2744)
        this.setName("rbmk_fuel_zfb_am_mix")
        this.setTexture("rbmk_fuel_zfb_am_mix")
    }
}
class DRX extends RBMKFuel {
    constructor() {
        super()

        this.setYield(1000000)
        this.setStats(1000, 10)
        this.setFunction(EnumBurnFunc.QUADRATIC)
        this.setHeat(0.1)
        this.setMeltingPoint(100000)
        this.setName("rbmk_fuel_drx")
        this.setTexture("rbmk_fuel_drx")
    }
}
class TEST extends RBMKFuel {
    constructor() {
        super()

        this.setYield(1000000)
        this.setStats(100)
        this.setFunction(EnumBurnFunc.EXPERIMENTAL)
        this.setHeat(1.0)
        this.setMeltingPoint(100000)
        this.setName("rbmk_fuel_test")
        this.setTexture("rbmk_fuel_test")
    }
}
var fuels = [NONE, UEU, MEU, HEU233, HEU235, THMEU, LEP, MEP, HEP239, HEP241, LEA, MEA, HEA241, HEA242, MEN, HEN, MOX, LES, MES, HES, LEAUS, HEAUS, PO210BE, RA226BE, PU238BE, BALEFIRE_GOLD, FLASHLEAD, BALEFIRE, ZFB_BISMUTH, ZFB_PU241, ZFB_AM_MIX, DRX, TEST]