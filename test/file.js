var expect = require("chai").expect,
    nodenetcdf = require("../build/Release/nodenetcdf.node");

describe('File', function() {
    describe('new', function() {
        it('should throw an error when file not found', function() {
            expect(function() {
                var file = new nodenetcdf.File("DOESNOTEXIST", "r");
            }).to.throw("No such file or directory");
        });

        it('should throw an error for wrong file mode', function() {
            expect(function() {
                var file = new nodenetcdf.File("test/testrh.nc", "WRONG");
            }).to.throw("Unknown file mode");
        });
    });

});
