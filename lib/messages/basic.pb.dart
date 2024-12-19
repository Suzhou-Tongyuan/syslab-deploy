// ignore_for_file: invalid_language_version_override

import 'dart:async';
import 'dart:typed_data';
import 'package:rinf/rinf.dart';

//
//  Generated code. Do not modify.
//  source: basic.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// [DART-SIGNAL]
class GenericDartSignal extends $pb.GeneratedMessage {
  factory GenericDartSignal({
    $core.String? text,
  }) {
    final $result = create();
    if (text != null) {
      $result.text = text;
    }
    return $result;
  }
  GenericDartSignal._() : super();
  factory GenericDartSignal.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GenericDartSignal.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'GenericDartSignal', package: const $pb.PackageName(_omitMessageNames ? '' : 'basic'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'text')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  GenericDartSignal clone() => GenericDartSignal()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  GenericDartSignal copyWith(void Function(GenericDartSignal) updates) => super.copyWith((message) => updates(message as GenericDartSignal)) as GenericDartSignal;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static GenericDartSignal create() => GenericDartSignal._();
  GenericDartSignal createEmptyInstance() => create();
  static $pb.PbList<GenericDartSignal> createRepeated() => $pb.PbList<GenericDartSignal>();
  @$core.pragma('dart2js:noInline')
  static GenericDartSignal getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GenericDartSignal>(create);
  static GenericDartSignal? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get text => $_getSZ(0);
  @$pb.TagNumber(1)
  set text($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasText() => $_has(0);
  @$pb.TagNumber(1)
  void clearText() => clearField(1);
}

/// [RUST-SIGNAL]
class GenericRustSignal extends $pb.GeneratedMessage {static final rustSignalStream =
    genericRustSignalController.stream.asBroadcastStream();

  factory GenericRustSignal({
    $core.String? text,
  }) {
    final $result = create();
    if (text != null) {
      $result.text = text;
    }
    return $result;
  }
  GenericRustSignal._() : super();
  factory GenericRustSignal.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GenericRustSignal.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'GenericRustSignal', package: const $pb.PackageName(_omitMessageNames ? '' : 'basic'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'text')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  GenericRustSignal clone() => GenericRustSignal()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  GenericRustSignal copyWith(void Function(GenericRustSignal) updates) => super.copyWith((message) => updates(message as GenericRustSignal)) as GenericRustSignal;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static GenericRustSignal create() => GenericRustSignal._();
  GenericRustSignal createEmptyInstance() => create();
  static $pb.PbList<GenericRustSignal> createRepeated() => $pb.PbList<GenericRustSignal>();
  @$core.pragma('dart2js:noInline')
  static GenericRustSignal getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GenericRustSignal>(create);
  static GenericRustSignal? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get text => $_getSZ(0);
  @$pb.TagNumber(1)
  set text($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasText() => $_has(0);
  @$pb.TagNumber(1)
  void clearText() => clearField(1);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');

extension GenericDartSignalExt on GenericDartSignal{
  void sendSignalToRust() {
    sendDartSignal(
      0,
      this.writeToBuffer(),
      Uint8List(0),
    );
  }
}

final genericRustSignalController = StreamController<RustSignal<GenericRustSignal>>();
