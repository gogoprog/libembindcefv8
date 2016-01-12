
//var test = require('./kludjs.js');

function isEven(x) {
	return x % 2 == 0;
}

test('Testing isEven', function() {
	ok(isEven(0), 'Zero is even');
	ok(isEven(1) == false, 'One is odd');
	ok(isEven(12), 'Twelve is even');
});


/*
var test = Module.AStruct();
console.log(test.floatMember);
console.log(test.intMember);
console.log(test.stringMember);

test = new Module.AStructContainer();
console.log(test.aInt);

console.log(test.aMember.floatMember);
console.log(test.aMember.intMember);
console.log(test.aMember.stringMember);

test.modifyMembers();

console.log(test.aMember.floatMember);
console.log(test.aMember.intMember);
console.log(test.aMember.stringMember);

test.aMethod();
test.aMethod1(1);
test.aMethod2(1, 2);

test = new Module.AStructContainer(6);
*/
