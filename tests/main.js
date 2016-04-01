
test('ValueObject - constructor0', function() {
    var o = Module.AStruct();

    ok(o.floatMember == 16, 'Float property');
    ok(o.intMember == 1024, 'Int property');
    ok(o.stringMember == "A sample string", 'String property');
});

test('ValueObject - constructor1', function() {
    var o = Module.AStruct(2);

    ok(o.floatMember == 32, 'Float property');
    ok(o.intMember == 2048, 'Int property');
    ok(o.stringMember == "A sample string", 'String property');
});

test('Class - constructor0', function() {
    var o = new Module.AStructContainer();

    ok(o.aInt == 128, 'Int property');
    ok(o.aMember.floatMember == 16, 'ValueObject property');
    ok(o.aMember.intMember == 1024, 'ValueObject property');
    ok(o.aMember.stringMember == "A sample string", 'ValueObject property');
});

test('Class - void methods', function() {
    var o = new Module.AStructContainer();

    ok(typeof o.aMethod() === "undefined", 'No argument');
    ok(typeof o.aMethod1(1) === "undefined", '1 argument');
    ok(typeof o.aMethod2(1, 2) === "undefined", '2 arguments');
    ok(typeof o.aMethod3(1, 2, 3) === "undefined", '3 arguments');
});

test('Class - modification', function() {
    var o = new Module.AStructContainer();

    ok(typeof o.modifyMembers() === "undefined", 'Modifier method call');
    ok(o.aMember.floatMember == 32, 'ValueObject property');
    ok(o.aMember.intMember == 2048, 'ValueObject property');
    ok(o.aMember.stringMember == "Another string", 'ValueObject property');
});

test('Class - result methods', function() {
    var o = new Module.AStructContainer();

	ok(o.resultMethod() === 128, 'No argument');
	ok(o.resultMethod1(4) === 4, '1 argument');
	ok(o.resultMethod2(4, 2) === 8, '2 arguments');
    ok(o.resultMethod3(4, 2, 3) === 24, '3 arguments');
});
