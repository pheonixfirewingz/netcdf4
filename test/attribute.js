var expect = require("chai").expect,
    nodenetcdf = require("../build/Release/nodenetcdf.node");

describe('Attribute', function() {
    describe('name', function() {
        it('should read group attribute name', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].attributes;
            expect(attributes["airport_dep"].name).to.equal("airport_dep");
        });
    });

    describe('value', function() {
        it('should read group attribute value', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].attributes;
            expect(attributes["airport_dep"].value).to.equal("FRA");
        });
    });

    describe('name', function() {
        it('should read variable attribute name', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].variables["air_press"].attributes;
            expect(attributes["name"].name).to.equal("name");
        });
    });

    describe('value', function() {
        it('should read variable attribute name', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].variables["air_press"].attributes;
            expect(attributes["name"].value).to.equal("air_pressure");
        });
    });

    describe('toJSON', function() {
        it('should serialize attribute without crashing', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].attributes;
            var attr = attributes["airport_dep"];
            
            // Explicitly call toJSON to test the V8 ToLocalChecked fix
            var json = attr.toJSON();
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('name', 'airport_dep');
            expect(json).to.have.property('value', 'FRA');
        });

        it('should handle variable attribute toJSON without crashing', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].variables["air_press"].attributes;
            var attr = attributes["name"];
            
            // Explicitly call toJSON to test the V8 ToLocalChecked fix
            var json = attr.toJSON();
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('name', 'name');
            expect(json).to.have.property('value', 'air_pressure');
        });

        it('should work with JSON.stringify', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].attributes;
            var attr = attributes["airport_dep"];
            
            // Test that JSON.stringify (which calls toJSON internally) works
            var jsonString = JSON.stringify(attr);
            var json = JSON.parse(jsonString);
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('name', 'airport_dep');
            expect(json).to.have.property('value', 'FRA');
        });

        it('should handle errors gracefully and return null value on type conversion errors', function() {
            var file = new nodenetcdf.File("test/test_hgroups.nc", "r");
            var attributes = file.root.subgroups["mozaic_flight_2012030419144751_ascent"].attributes;
            
            // Iterate through all attributes to find any that might have type issues
            // The toJSON method should handle these gracefully
            for (var attrName in attributes) {
                var attr = attributes[attrName];
                
                // This should not crash even if there are type conversion errors
                var json = attr.toJSON();
                
                expect(json).to.be.an('object');
                expect(json).to.have.property('name');
                expect(json).to.have.property('value');
                // Value should either be a valid value or null if there was an error
                expect(json.value === null || typeof json.value !== 'undefined').to.be.true;
            }
        });
    });
});
