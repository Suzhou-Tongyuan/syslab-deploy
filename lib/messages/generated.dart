import './basic.pb.dart' as basic;
// ignore_for_file: unused_import

import 'dart:typed_data';
import 'package:rinf/rinf.dart';

final rustSignalHandlers = <int, void Function(Uint8List, Uint8List)>{
1: (Uint8List messageBytes, Uint8List binary) {
  final message = basic.GenericRustSignal.fromBuffer(messageBytes);
  final rustSignal = RustSignal(
    message,
    binary,
  );
  basic.genericRustSignalController.add(rustSignal);
},
};

void assignRustSignal(int messageId, Uint8List messageBytes, Uint8List binary) {
  rustSignalHandlers[messageId]!(messageBytes, binary);
}
