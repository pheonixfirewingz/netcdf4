var expect = require("chai").expect,
    nodenetcdf = require("../build/Release/nodenetcdf.node");

describe('JSON Serialization', function() {
    var file;
    
    beforeEach(function() {
        file = new nodenetcdf.File("test/testrh.nc", "r");
    });
    
    afterEach(function() {
        file.close();
    });
    
    describe('Dimension', function() {
        it('should serialize dimension to JSON', function() {
            var dim = file.root.dimensions['dim1'];
            var json = JSON.parse(JSON.stringify(dim));
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('id');
            expect(json).to.have.property('name', 'dim1');
            expect(json).to.have.property('length', 10000);
        });
        
        it('should have all required properties in JSON', function() {
            var dim = file.root.dimensions['dim1'];
            var json = JSON.parse(JSON.stringify(dim));
            
            expect(json.id).to.be.a('number');
            expect(json.name).to.be.a('string');
            expect(json.length).to.be.a('number');
        });
    });
    
    describe('Attribute', function() {
        it('should serialize group attribute to JSON', function() {
            var attributes = file.root.attributes;
            for (var attrName in attributes) {
                var attr = attributes[attrName];
                var json = JSON.parse(JSON.stringify(attr));
                
                expect(json).to.be.an('object');
                expect(json).to.have.property('name');
                expect(json).to.have.property('value');
                return; // Test first attribute
            }
        });
        
        it('should serialize variable attribute to JSON', function() {
            var variables = file.root.variables;
            for (var varName in variables) {
                var variable = variables[varName];
                var varAttrs = variable.attributes;
                for (var attrName in varAttrs) {
                    var attr = varAttrs[attrName];
                    var json = JSON.parse(JSON.stringify(attr));
                    
                    expect(json).to.be.an('object');
                    expect(json).to.have.property('name');
                    expect(json).to.have.property('value');
                    return; // Test first attribute
                }
            }
        });
        
        it('should not serialize attribute values as null', function() {
            // Check all attributes in file don't have null values when serialized
            var jsonStr = JSON.stringify(file);
            var nullMatches = jsonStr.match(/"value":\s*null/g);
            var nullCount = nullMatches ? nullMatches.length : 0;
            
            expect(nullCount).to.equal(0, 'Found ' + nullCount + ' null attribute values in JSON');
        });
        
        it('should convert TypedArray values to regular arrays in JSON', function() {
            // Find an attribute with a TypedArray value
            var variables = file.root.variables;
            for (var varName in variables) {
                var variable = variables[varName];
                var varAttrs = variable.attributes;
                for (var attrName in varAttrs) {
                    var attr = varAttrs[attrName];
                    
                    // Check if value is a TypedArray
                    if (attr.value && typeof attr.value === 'object' && 
                        attr.value.constructor && attr.value.constructor.name.includes('Array') &&
                        !Array.isArray(attr.value)) {
                        
                        // It's a TypedArray, test it converts to regular array
                        var json = JSON.parse(JSON.stringify(attr));
                        
                        // In JSON, it should be a regular array
                        expect(Array.isArray(json.value)).to.be.true;
                        return;
                    }
                }
            }
        });
    });
    
    describe('Variable', function() {
        it('should serialize variable to JSON', function() {
            var variable = file.root.variables['var1'];
            var json = JSON.parse(JSON.stringify(variable));
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('id');
            expect(json).to.have.property('name', 'var1');
            expect(json).to.have.property('type', 'double');
        });
        
        it('should have all required properties in JSON', function() {
            var variable = file.root.variables['var1'];
            var json = JSON.parse(JSON.stringify(variable));
            
            expect(json.id).to.be.a('number');
            expect(json.name).to.be.a('string');
            expect(json.type).to.be.a('string');
        });
    });
    
    describe('Group', function() {
        it('should serialize group to JSON', function() {
            var group = file.root;
            var json = JSON.parse(JSON.stringify(group));
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('id');
            expect(json).to.have.property('name');
            expect(json).to.have.property('fullname');
        });
        
        it('should have all required properties in JSON', function() {
            var group = file.root;
            var json = JSON.parse(JSON.stringify(group));
            
            expect(json.id).to.be.a('number');
            expect(json.name).to.be.a('string');
            expect(json.fullname).to.be.a('string');
        });
        
        it('should recursively serialize all children', function() {
            var group = file.root;
            var json = JSON.parse(JSON.stringify(group));
            
            expect(json).to.have.property('dimensions');
            expect(json).to.have.property('variables');
            expect(json).to.have.property('attributes');
            expect(json).to.have.property('subgroups');
        });
    });
    
    describe('File', function() {
        it('should serialize file to JSON', function() {
            var json = JSON.parse(JSON.stringify(file));
            
            expect(json).to.be.an('object');
            expect(json).to.have.property('id');
            expect(json).to.have.property('name');
            expect(json).to.have.property('fullname');
        });
        
        it('should recursively serialize entire file structure', function() {
            var json = JSON.parse(JSON.stringify(file));
            
            // File should serialize as root group with all children as arrays
            expect(json).to.have.property('dimensions');
            expect(json).to.have.property('variables');
            expect(json).to.have.property('attributes');
            expect(json).to.have.property('subgroups');
            expect(json.variables).to.be.an('array');
            expect(json.dimensions).to.be.an('array');
            expect(json.attributes).to.be.an('array');
            expect(json.subgroups).to.be.an('array');
        });
    });
});
