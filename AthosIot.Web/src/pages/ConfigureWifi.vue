<template>
  <div class="content">
    <div class="container-fluid">
      <div class="row">
        <div class="col-md-8">
          <edit-wifi-form></edit-wifi-form> 
        </div>
        <div class="col-md-4">
        </div>
      </div>
    </div>
  </div>
</template>
<script>
  import EditWifiForm from './Admin/EditWifiForm.vue'

  export default {
    components: {
      EditWifiForm
    }
  }

</script>
<style>

</style>
